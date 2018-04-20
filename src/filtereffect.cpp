#include <algorithm>

#include "NE10.h"
#include "filtereffect.h"

FilterEffect::FilterEffect(const std::vector<float>& impulseResponse)
    : numBlocksReceived(0)
{
    // TODO: Fix MiniConvolver not being copyable! (#3)
    convolvers.reserve(20);

    // Split the impulse response in blocks
    auto it = impulseResponse.begin();
    bool isEvenBlock = true;
    unsigned int blockSize = Constants::BLOCK_SIZE;
    unsigned int delay = Constants::BLOCK_SIZE;
    unsigned int largestBlockSize = Constants::BLOCK_SIZE;

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
        convolvers.emplace_back(impulseFragment, delay, blockSize >= 16000);

        // Update parameters
        largestBlockSize = blockSize;
        if (!isEvenBlock)
            blockSize *= 2; // only double blocksize every two segments
        isEvenBlock = !isEvenBlock;
        delay += impulseFragment.size();
    }

    // Make input buffer big enough to remember enough blocks
    inputBuffer.resize(largestBlockSize);
}

void FilterEffect::push(const std::array<float, Constants::BLOCK_SIZE>& data)
{
    // New block received
    ++numBlocksReceived;

    // Remove first block and add new block to buffer
    inputBuffer.erase(inputBuffer.begin(), inputBuffer.begin() + Constants::BLOCK_SIZE);
    inputBuffer.insert(inputBuffer.end(), data.begin(), data.end());

    // Calculate output
    std::vector<float> output(Constants::BLOCK_SIZE);

    for (MiniConvolver& conv : convolvers)
    {
        std::vector<float> result = conv.getNextBlock();
        ne10_add_float(output.data(), output.data(), result.data(), Constants::BLOCK_SIZE);
    }

    // Check all convolvers to see if they should recalculate samples
    for (MiniConvolver& conv : convolvers)
    {
        // Check if this convolver has enough samples
        if ((numBlocksReceived * Constants::BLOCK_SIZE) % conv.getBlockSize() == 0)
        {
            conv.calculate(std::vector<float>(inputBuffer.end() - conv.getBlockSize(), inputBuffer.end()));
        }
    }

    std::array<float, Constants::BLOCK_SIZE> outputArray;
    std::copy_n(output.begin(), Constants::BLOCK_SIZE, outputArray.begin());
    generate(outputArray);
}

FilterEffect::MiniConvolver::MiniConvolver(const std::vector<float>& impulseResponse, unsigned int delay, bool inBackground)
    : conv(impulseResponse, impulseResponse.size()), blockSize(impulseResponse.size()), outputBuffer(delay), inBackground(inBackground), mutex(new std::mutex()), cond_var(new std::condition_variable())
{
}

void FilterEffect::MiniConvolver::calculate(const std::vector<float>& input)
{
    if (!inBackground)
    {
        std::vector<float> result = conv.process(input);
        outputBuffer.insert(outputBuffer.end(), result.begin(), result.end());
    }
    else
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
}

std::vector<float> FilterEffect::MiniConvolver::getNextBlock()
{
    if (!inBackground)
    {
        std::vector<float> block(outputBuffer.begin(), outputBuffer.begin() + Constants::BLOCK_SIZE);
        outputBuffer.erase(outputBuffer.begin(), outputBuffer.begin() + Constants::BLOCK_SIZE);
        return block;
    }
    else
    {
        // wait until new data is available
        std::unique_lock<std::mutex> lock(*mutex);
        cond_var->wait(lock, [this]{ return !outputBuffer.empty(); });

        std::vector<float> block(outputBuffer.begin(), outputBuffer.begin() + Constants::BLOCK_SIZE);
        outputBuffer.erase(outputBuffer.begin(), outputBuffer.begin() + Constants::BLOCK_SIZE);
        return block;
    }
}

