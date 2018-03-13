#include <algorithm>
#include <iterator>
#include <iostream>

#include "convolver.h"

Convolver::Convolver(const std::vector<float>& impulseResponse)
    : Processor(1), overlap(impulseResponse.size() - 1)
{
    if (overlap > BLOCK_SIZE)
        throw std::invalid_argument("The impulse response size must be <= blockSize + 1");

    period = nextPowerOfTwo(BLOCK_SIZE + overlap);
    config = ne10_fft_alloc_r2c_float32(period);

    // set last block to all zeros
    lastBlock = std::make_shared<std::vector<float>>(BLOCK_SIZE, 0.0f);

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

std::shared_ptr<std::vector<float>> Convolver::process(const std::vector<std::shared_ptr<std::vector<float>>>& data)
{
    if (data[0]->size() != BLOCK_SIZE)
        throw std::invalid_argument("Block must have blockSize elements");

    std::vector<float> inputSegment(period);
    std::copy_n(lastBlock->begin() + BLOCK_SIZE - overlap, overlap, inputSegment.begin());
    std::copy(data[0]->begin(), data[0]->end(), inputSegment.begin() + overlap);

    std::vector<Complex> inputFourier(period / 2 + 1);
    ne10_fft_r2c_1d_float32(inputFourier.data(), inputSegment.data(), config);

    std::vector<Complex> outputFourier = multiplyComplex(inputFourier, frequencyResponse);

    std::vector<float> output(period);
    ne10_fft_c2r_1d_float32(output.data(), outputFourier.data(), config);

    lastBlock = data[0];
    return std::make_shared<std::vector<float>>(output.begin() + overlap, output.begin() + overlap + BLOCK_SIZE);
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

