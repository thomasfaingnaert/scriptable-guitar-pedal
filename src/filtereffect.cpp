#include "NE10.h"
#include "filtereffect.h"

FilterEffect::FilterEffect(const std::vector<float>& impulseResponse)
    : Processor(1), numBlocksReceived(0)
{
    // TODO: Fix MiniConvolver not being copyable! (#3)
    convolvers.reserve(20);

    // Split the impulse response in blocks
    auto it = impulseResponse.begin();
    bool isEvenBlock = true;
    unsigned int blockSize = BLOCK_SIZE;
    unsigned int delay = BLOCK_SIZE;
    unsigned int largestBlockSize = BLOCK_SIZE;

    while (it != impulseResponse.end())
    {
        // Create impulse response fragment
        std::vector<float> impulseFragment;
        if (it + blockSize < impulseResponse.end())
        {
            impulseFragment = std::vector<float>(it, it + blockSize);
            it += blockSize;
        }
        else
        {
            impulseFragment = std::vector<float>(it, impulseResponse.end());
            impulseFragment.resize(blockSize);
            it = impulseResponse.end();
        }

        // Create new mini-convolver
        convolvers.emplace_back(impulseFragment, delay);

        // Update parameters
        largestBlockSize = blockSize;
        if (!isEvenBlock)
            blockSize *= 2; // only double blocksize every two segments
        isEvenBlock = !isEvenBlock;
        delay += impulseFragment.size();
    }

    // Make input buffer big enough to remember enough blocks
    inputBuffer.resize(largestBlockSize);

#if 0
    // Make input buffer big enough to remember enough blocks
    inputBuffer.resize(2 * BLOCK_SIZE);

    // TODO: Fix MiniConvolver not being copyable! (#3)
    convolvers.reserve(10);

    // Make convolvers
    std::vector<float> impulse1(BLOCK_SIZE);
    std::vector<float> impulse2(BLOCK_SIZE);
    std::vector<float> impulse3(2 * BLOCK_SIZE);
    std::vector<float> impulse4(2 * BLOCK_SIZE);

    impulse4[0] = 1;

    convolvers.emplace_back(impulse1, BLOCK_SIZE * 1);
    convolvers.emplace_back(impulse2, BLOCK_SIZE * 2);
    convolvers.emplace_back(impulse3, BLOCK_SIZE * 3);
    convolvers.emplace_back(impulse4, BLOCK_SIZE * 5);
#endif
}

std::shared_ptr<std::vector<float>> FilterEffect::process(const std::vector<std::shared_ptr<std::vector<float>>> &data)
{
    // New block received
    ++numBlocksReceived;

    // Remove first block and add new block to buffer
    inputBuffer.erase(inputBuffer.begin(), inputBuffer.begin() + BLOCK_SIZE);
    inputBuffer.insert(inputBuffer.end(), data[0]->begin(), data[0]->end());

    // Calculate output
    std::vector<float> output(BLOCK_SIZE);

    for (MiniConvolver& conv : convolvers)
    {
        std::vector<float> result = conv.getNextBlock();
        ne10_add_float(output.data(), output.data(), result.data(), BLOCK_SIZE);
    }

    // Check all convolvers to see if they should recalculate samples
    for (MiniConvolver& conv : convolvers)
    {
        // Check if this convolver has enough samples
        if ((numBlocksReceived * BLOCK_SIZE) % conv.getBlockSize() == 0)
        {
            conv.calculate(std::vector<float>(inputBuffer.end() - conv.getBlockSize(), inputBuffer.end()));
        }
    }

    return std::make_shared<std::vector<float>>(output);
}

FilterEffect::MiniConvolver::MiniConvolver(const std::vector<float>& impulseResponse, unsigned int delay)
    : conv(impulseResponse, impulseResponse.size()), blockSize(impulseResponse.size()), outputBuffer(delay), mutex(new std::mutex()), cond_var(new std::condition_variable())
{
}

void FilterEffect::MiniConvolver::calculate(const std::vector<float>& input)
{
    if (thread.joinable())
        thread.join();

    thread = std::thread([this,input]
            {
                std::vector<float> result = conv.process(input);
                {
                    std::unique_lock<std::mutex> lock(*mutex);
                    outputBuffer.insert(outputBuffer.end(), result.begin(), result.end());
                    lock.unlock();
                    cond_var->notify_one();
                }
            });
}

std::vector<float> FilterEffect::MiniConvolver::getNextBlock()
{
    // wait until new data is available
    std::unique_lock<std::mutex> lock(*mutex);
    cond_var->wait(lock, [this]{ return !outputBuffer.empty(); });

    std::vector<float> block(outputBuffer.begin(), outputBuffer.begin() + BLOCK_SIZE);
    outputBuffer.erase(outputBuffer.begin(), outputBuffer.begin() + BLOCK_SIZE);
    return block;
}

