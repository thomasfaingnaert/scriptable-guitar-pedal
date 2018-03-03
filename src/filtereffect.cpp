#include "filtereffect.h"

void FilterEffect::setImpulseResponse(std::vector<Sample> impulseResponse)
{
    // Resize input to power of two
    impulseResponse.resize(nextPowerOfTwo(impulseResponse.size()));

    // Resize output to size of FFT of input
    frequencyResponse.resize(impulseResponse.size() / 2 + 1);

    // Ne10 FFT configuration for impulse response -> frequency reponse FFT
    ne10_fft_r2c_cfg_float32_t impulseConfig = ne10_fft_alloc_r2c_float32(impulseResponse.size());

    // Perform FFT
    ne10_fft_r2c_1d_float32(frequencyResponse.data(), impulseResponse.data(), impulseConfig);

    // Cleanup
    ne10_fft_destroy_r2c_float32(impulseConfig);
}

void FilterEffect::setFrequencyResponse(const std::vector<Complex>& frequencyResponse)
{
    this->frequencyResponse = frequencyResponse;
}

std::vector<Complex> FilterEffect::getFrequencyResponse() const
{
    return frequencyResponse;
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

