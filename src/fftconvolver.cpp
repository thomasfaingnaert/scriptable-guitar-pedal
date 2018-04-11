#include "fftconvolver.h"
#include <cmath>
#include <stdexcept>

FFTConvolver::FFTConvolver(const std::vector<float>& impulseResponse, unsigned int blockSize)
    : blockSize(blockSize)
{
    overlap = impulseResponse.size() - 1;
    period = nextPowerOfTwo(blockSize + overlap);

    config = ne10_fft_alloc_r2c_float32(period);

    frequencyResponse = fft(impulseResponse, period, config);
    inputBuffer = std::vector<float>(overlap, 0.0);
}

FFTConvolver::~FFTConvolver()
{
    ne10_fft_destroy_r2c_float32(config);
}

std::vector<float> FFTConvolver::process(const std::vector<float>& input)
{
    if (input.size() != blockSize)
        throw std::invalid_argument("Input should be blockSize samples long.");

    // Add new input block
    inputBuffer.insert(inputBuffer.end(), input.begin(), input.end());

    // get input segment = overlap samples from previous block + all samples from current block
    std::vector<float> inputSegment(inputBuffer.begin(), inputBuffer.begin() + blockSize + overlap);

    // fourier transform of input segment
    std::vector<Complex> inputFourier = fft(inputSegment, period, config);

    // multiply by frequency response to get output FT
    std::vector<Complex> outputFourier = multiplyComplex(inputFourier, frequencyResponse);

    // inverse transform
    std::vector<float> output = ifft(outputFourier, period, config);

    // remove block from input buffer
    inputBuffer.erase(inputBuffer.begin(), inputBuffer.begin() + blockSize);

    // return only the needed samples from output
    return std::vector<float>(output.begin() + overlap, output.begin() + overlap + blockSize);
}

unsigned int FFTConvolver::nextPowerOfTwo(unsigned int n) const
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

