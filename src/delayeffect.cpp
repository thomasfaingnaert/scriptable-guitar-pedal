#include <algorithm>
#include <iostream>

#include "NE10.h"
#include "delayeffect.h"

DelayEffect::DelayEffect(float mainCoeff, const std::vector<unsigned int>& delays, const std::vector<float>& coeffs)
    : Processor(1), mainCoeff(mainCoeff), delays(delays), coeffs(coeffs)
{
    if (delays.size() != coeffs.size())
        throw std::invalid_argument("Delays and Coeffs should have the same size");

    const unsigned int max_delay = *std::max_element(delays.cbegin(), delays.cend());
    auto zero = std::make_shared<std::vector<float>>(BLOCK_SIZE);
    buffer = std::deque<std::shared_ptr<std::vector<float>>>(max_delay, zero);
}

std::shared_ptr<std::vector<float>> DelayEffect::process(const std::vector<std::shared_ptr<std::vector<float>>>& data)
{
    auto result = std::make_shared<std::vector<float>>(BLOCK_SIZE);

    // zero delay line
    ne10_mulc_float(result->data(), data[0]->data(), mainCoeff, BLOCK_SIZE);

    // temporary buffer
    std::vector<float> tmp(BLOCK_SIZE);

    // add every delay line
    for (unsigned int line = 0; line < delays.size(); ++line)
    {
        const unsigned int delay = delays[line];
        const float coeff = coeffs[line];

        ne10_mulc_float(tmp.data(), buffer[delay - 1]->data(), coeff, BLOCK_SIZE);
        ne10_add_float(result->data(), result->data(), tmp.data(), BLOCK_SIZE);
    }

    // add new block and remove old one
    buffer.push_front(data[0]);
    buffer.pop_back();

    return result;
}

