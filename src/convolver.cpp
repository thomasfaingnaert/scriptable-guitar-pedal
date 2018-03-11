#include <algorithm>
#include <iterator>
#include <iostream>

#include "convolver.h"

Convolver::Convolver(const std::vector<float>& impulseResponse, unsigned int blockSize)
    : blockSize(blockSize), overlap(impulseResponse.size() - 1)
{
    if (overlap > blockSize)
        throw std::invalid_argument("The impulse response size must be <= blockSize + 1");

    period = nextPowerOfTwo(blockSize + overlap);
    config = ne10_fft_alloc_r2c_float32(period);

    // set last block to all zeros
    lastBlock = std::make_shared<std::vector<float>>(blockSize, 0.0f);

    // calculate frequency response
    frequencyResponse = fft(impulseResponse, period, config);
}

Convolver::~Convolver()
{
    if (config)
    {
        ne10_fft_destroy_r2c_float32(config);
    }
}

std::shared_ptr<std::vector<float>> Convolver::process(const std::shared_ptr<std::vector<float>>& block)
{
    if (block->size() != blockSize)
        throw std::invalid_argument("Block must have blockSize elements");

    std::vector<float> inputSegment(blockSize + overlap);
    std::copy_n(lastBlock->begin() + blockSize - overlap, overlap, inputSegment.begin());
    std::copy(block->begin(), block->end(), inputSegment.begin() + overlap);

    std::vector<Complex> inputFourier = fft(inputSegment, period, config);

    std::vector<Complex> outputFourier = multiplyComplex(inputFourier, frequencyResponse);

    std::vector<float> output = ifft(outputFourier, period, config);
    lastBlock = block;
    return std::make_shared<std::vector<float>>(output.begin() + overlap, output.begin() + overlap + blockSize);
}

unsigned int Convolver::nextPowerOfTwo(unsigned int n) const
{
    if (sizeof(n) != 4)
    {
        return 1 << static_cast<unsigned int>(std::ceil(std::log(n)/std::log(2)));
    }
    else
    {
        // see https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
        --n;

        n |= n >> 1;
        n |= n >> 2;
        n |= n >> 4;
        n |= n >> 8;
        n |= n >> 16;

        ++n;

        return n;
    }
}

