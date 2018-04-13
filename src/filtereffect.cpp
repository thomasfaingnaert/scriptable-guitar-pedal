#include "filtereffect.h"
#include <iostream>
#include <iterator>

FilterEffect::FilterEffect()
    : Processor(1), numBlocksReceived(0)
{
    // Make input buffer big enough to remember enough blocks
    inputBuffer.resize(2 * BLOCK_SIZE);

    convolvers.reserve(10);

    // Make convolvers
    std::vector<float> impulse1(BLOCK_SIZE);
    std::vector<float> impulse2(BLOCK_SIZE);
    std::vector<float> impulse3(2 * BLOCK_SIZE);
    std::vector<float> impulse4(2 * BLOCK_SIZE);

    impulse3[0] = 1;

    convolvers.emplace_back(impulse1);
    convolvers.emplace_back(impulse2);
    convolvers.emplace_back(impulse3);
    convolvers.emplace_back(impulse4);
}

std::shared_ptr<std::vector<float>> FilterEffect::process(const std::vector<std::shared_ptr<std::vector<float>>> &data)
{
    // New block received
    ++numBlocksReceived;

    // Remove first block and add new block to buffer
    inputBuffer.erase(inputBuffer.begin(), inputBuffer.begin() + BLOCK_SIZE);
    inputBuffer.insert(inputBuffer.end(), data[0]->begin(), data[0]->end());

    // Output
    std::cout << "Block " << numBlocksReceived - 1 << ":\n";

#if 0
    for (MiniConvolver& conv : convolvers)
    {
        std::cout << "convolver:\n";
        std::vector<float> res = conv.getLastResult();
        std::copy(res.begin(), res.end(), std::ostream_iterator<float>(std::cout, " "));
        std::cout << "\n";
    }
#endif

    // Calculate output
    std::vector<float> output = convolvers[0].getLastResult();

    for (int i = 0; i < convolvers[1].getPreviousResult().size(); ++i)
    {
        output[i] += convolvers[1].getPreviousResult()[i];
    }

    for (int i = 0; i < BLOCK_SIZE; ++i)
    {
        std::cout << convolvers[2].getLastResult().at(convolvers[2].index) << " ";
        output[i] += convolvers[2].getLastResult().at(convolvers[2].index++);
    }

    std::cout << "output:\n";
    for (float f : output)
    {
        std::cout << std::round(f) << " ";
    }
    std::cout << "\n\n";

    // Check all convolvers to see if they should recalculate samples
    for (MiniConvolver& conv : convolvers)
    {
        // Check if this convolver has enough samples
        if ((numBlocksReceived * BLOCK_SIZE) % conv.getBlockSize() == 0)
        {
            conv.calculate(std::vector<float>(inputBuffer.end() - conv.getBlockSize(), inputBuffer.end()));
        }
    }

    return data[0];
}

FilterEffect::MiniConvolver::MiniConvolver(const std::vector<float>& impulseResponse)
    : conv(impulseResponse, impulseResponse.size()), blockSize(impulseResponse.size()), result(impulseResponse.size()), prevResult(impulseResponse.size()), index(0)
{
}

void FilterEffect::MiniConvolver::calculate(const std::vector<float>& input)
{
    std::swap(result, prevResult);
    result = conv.process(input);
    index = 0;
}

