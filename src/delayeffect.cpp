#include <algorithm>
#include <iostream>
#include <iterator>

#include "NE10.h"
#include "delayeffect.h"

DelayEffect::DelayEffect(float mainCoeff, const std::vector<unsigned int>& delays, const std::vector<float>& coeffs)
    : mainCoeff(mainCoeff), delays(delays), coeffs(coeffs), inputBuffer(0, 0)
{
    if (delays.size() != coeffs.size())
        throw std::invalid_argument("Delays and Coeffs should have the same size");

    unsigned int maxDelay;
    if (!delays.empty())
        maxDelay = *std::max_element(delays.cbegin(), delays.cend());
    else
        maxDelay = 0;

    // Create buffer with enough capacity to remember one block, plus the correct amount of samples for the longest delay line
    inputBuffer = BlockBuffer<float>(1 + std::ceil(static_cast<float>(maxDelay) / Constants::BLOCK_SIZE), Constants::BLOCK_SIZE);
}

void DelayEffect::push(const std::array<float, Constants::BLOCK_SIZE>& data)
{
    // Add block to input buffer
    inputBuffer.push_back(data.begin(), data.end());

    // Reserve space for result
    std::array<float, Constants::BLOCK_SIZE> result;

    // Zero delay line
    ne10_mulc_float(result.data(), const_cast<float*>(data.data()), mainCoeff, Constants::BLOCK_SIZE);

    // Other delay lines
    for (std::size_t i = 0; i < delays.size(); ++i)
    {
        float* source = inputBuffer.end() - (Constants::BLOCK_SIZE + delays[i]);
        ne10_mlac_float(result.data(), result.data(), source, coeffs[i], Constants::BLOCK_SIZE);
    }

    generate(result);
}

