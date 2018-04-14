#include "directformconvolver.h"

#include <stdexcept>

DirectFormConvolver::DirectFormConvolver(const std::vector<float>& impulseResponse, unsigned int blockSize)
    : blockSize(blockSize), taps(impulseResponse.rbegin(), impulseResponse.rend())
{
    state.resize(impulseResponse.size() + blockSize - 1);
    if (ne10_fir_init_float(&config, impulseResponse.size(), taps.data(), state.data(), blockSize) != NE10_OK)
        throw std::runtime_error("Could not initialise FIR float configuration");
}

std::vector<float> DirectFormConvolver::process(const std::vector<float>& input)
{
    if (input.size() != blockSize)
        throw std::invalid_argument("Input should be blockSize samples long.");

    std::vector<float> result(blockSize);

    ne10_fir_float(&config, const_cast<float*>(input.data()), result.data(), blockSize);

    return result;
}

