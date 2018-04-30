#include "frequencydelayline.h"
#include <stdexcept>

FrequencyDelayLine::FrequencyDelayLine(unsigned int blockSize, const std::vector<std::vector<float>>& impulseResponses)
    : N(blockSize), PERIOD(2*blockSize), OVERLAP(blockSize), COMPLEX_SIZE(blockSize + 8), inputBuffer(2, blockSize), frequencyBuffer(impulseResponses.size(), blockSize + 8), inputFourier(blockSize + 8), outputFourier(blockSize + 8), output(2*blockSize)
{
    // Only use powers of two as blocksizes
    if (!isPowerOfTwo(blockSize))
        throw std::invalid_argument("Blocksize should be power of two");

    // COMPLEX_SIZE restrictions
    // TODO: Fix complex multiply not working if size % 8 != 0
    if ((COMPLEX_SIZE % 8) != 0)
        throw std::invalid_argument("Complex size should be divisible by 8");

    // Allocate configuration for Ne10 FFT
    config = ne10_fft_alloc_r2c_float32(PERIOD);

    for (const auto& impulseResponse : impulseResponses)
    {
        // Check size of impulse response
        if (impulseResponse.size() != N)
            throw std::invalid_argument("Impulse should have size blockSize");

        // Resize impulseResponses to PERIOD samples by adding zeros
        std::vector<float> impulseResized(PERIOD);
        std::copy(impulseResponse.begin(), impulseResponse.end(), impulseResized.begin());

        // Calculate frequency responses
        frequencyResponses.emplace_back(COMPLEX_SIZE);
        ne10_fft_r2c_1d_float32(frequencyResponses.back().data(), impulseResized.data(), config);
    }
}

FrequencyDelayLine::~FrequencyDelayLine()
{
    // TODO: Find a clean way to handle this
    //ne10_fft_destroy_r2c_float32(config);
}
