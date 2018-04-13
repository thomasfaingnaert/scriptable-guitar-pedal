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

    impulse4[0] = 1;

    convolvers.emplace_back(impulse1, BLOCK_SIZE * 1);
    convolvers.emplace_back(impulse2, BLOCK_SIZE * 2);
    convolvers.emplace_back(impulse3, BLOCK_SIZE * 3);
    convolvers.emplace_back(impulse4, BLOCK_SIZE * 5);
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

    // Calculate output
    std::vector<float> output(BLOCK_SIZE);

    for (MiniConvolver& conv : convolvers)
    {
        std::vector<float> result = conv.getNextBlock();
        for (unsigned int i = 0; i < BLOCK_SIZE; ++i)
        {
            output[i] += result[i];
        }
    }

    std::cout << "Result:\n";
    for (float f : output)
        std::cout << std::round(f) << " ";
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

FilterEffect::MiniConvolver::MiniConvolver(const std::vector<float>& impulseResponse, unsigned int delay)
    : conv(impulseResponse, impulseResponse.size()), blockSize(impulseResponse.size()), outputBuffer(delay)
{
}

void FilterEffect::MiniConvolver::calculate(const std::vector<float>& input)
{
    std::vector<float> result = conv.process(input);
    outputBuffer.insert(outputBuffer.end(), result.begin(), result.end());
}

std::vector<float> FilterEffect::MiniConvolver::getNextBlock()
{
    std::vector<float> block(outputBuffer.begin(), outputBuffer.begin() + BLOCK_SIZE);
    outputBuffer.erase(outputBuffer.begin(), outputBuffer.begin() + BLOCK_SIZE);
    return block;
}

