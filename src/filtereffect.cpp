#include <iostream>
#include <stdexcept>

#include "filtereffect.h"

FilterEffect::~FilterEffect()
{
    if (config)
    {
        ne10_fft_destroy_r2c_float32(config);
    }
}

void FilterEffect::setImpulseResponse(const std::vector<Sample>& impulseResponse)
{
    // set parameters
    overlap = impulseResponse.size() - 1;
    period = nextPowerOfTwo(4 * overlap);
    blockSize = period - overlap;

    // add zeros before first block
    inputBuffer.insert(inputBuffer.begin(), overlap, 0);

    // delete old config
    if (config)
    {
        ne10_fft_destroy_r2c_float32(config);
    }

    // create new config
    config = ne10_fft_alloc_r2c_float32(period);

    // calculate frequency reponse
    frequencyResponse = fft(impulseResponse, period, config);
}

std::vector<Complex> FilterEffect::getFrequencyResponse() const
{
    return frequencyResponse;
}

unsigned int FilterEffect::getBlockSize() const
{
    return blockSize;
}

void FilterEffect::addInputBlock(const std::vector<Sample>& block)
{
    if (block.size() != blockSize)
    {
        throw std::invalid_argument("Block must have blockSize elements.");
    }

    inputBuffer.insert(inputBuffer.end(), block.begin(), block.end());
}

std::vector<Sample> FilterEffect::getOutputBlock()
{
    // get input segment = overlap samples from previous block + all samples from current block
    std::vector<Sample> inputSegment(inputBuffer.begin(), inputBuffer.begin() + blockSize + overlap);

    // fourier transform of input segment
    std::vector<Complex> inputFourier = fft(inputSegment, period, config);

    // multiply by frequency response to get output FT
    std::vector<Complex> outputFourier = multiplyComplex(inputFourier, frequencyResponse);

    // inverse transform
    std::vector<Sample> output = ifft(outputFourier, period, config);

    // remove unneeded samples
    output.erase(output.begin(), output.begin() + overlap);

    return output;
}

unsigned int FilterEffect::nextPowerOfTwo(unsigned int n) const
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

