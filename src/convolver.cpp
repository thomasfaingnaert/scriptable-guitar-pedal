#include <algorithm>
#include <iterator>
#include <iostream>

#include "convolver.h"

Convolver::Convolver(const std::vector<float>& impulseResponse)
    : Processor(1)
{
    // Split impulse response in blocks of increasing size
    std::size_t blockSize = BLOCK_SIZE;
    auto it = impulseResponse.begin();

    while (it != impulseResponse.end())
    {
        std::vector<float> block(blockSize);

        if (std::distance(it, impulseResponse.end()) >= blockSize)
        {
            // At least one block remaining
            std::copy(it, it + blockSize, block.begin());
            it += blockSize;
        }
        else
        {
            // Last block
            std::copy(it, impulseResponse.end(), block.begin());
            it = impulseResponse.end();
        }

        ne10_fft_r2c_cfg_float32_t config = ne10_fft_alloc_r2c_float32(blockSize);
        frequencyResponses.emplace_back(blockSize / 2 + 1);
        ne10_fft_r2c_1d_float32(frequencyResponses.back().data(), block.data(), config);
        configs.push_back(config);

        if (blockSize <= MAX_BLOCK_SIZE)
            blockSize *= 2;
    }
}

Convolver::~Convolver()
{
    // Clean up all FFT configs
    for (ne10_fft_r2c_cfg_float32_t config : configs)
    {
        ne10_fft_destroy_r2c_float32(config);
    }
}

std::shared_ptr<std::vector<float>> Convolver::process(const std::vector<std::shared_ptr<std::vector<float>>>& data)
{
}

