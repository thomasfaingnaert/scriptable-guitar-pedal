#ifndef FFTCONVOLVER_H_DLHBFNNX
#define FFTCONVOLVER_H_DLHBFNNX

#include <vector>

#include "NE10.h"
#include "dspmath.h"

class FFTConvolver
{
    public:
        FFTConvolver(const std::vector<float>& impulseResponse, unsigned int blockSize);
        ~FFTConvolver();
        std::vector<float> process(const std::vector<float>& input);

    private:
        unsigned int blockSize;
        unsigned int overlap;
        unsigned int period;
        ne10_fft_r2c_cfg_float32_t config;
        std::vector<float> inputBuffer;
        std::vector<Complex> frequencyResponse;

        unsigned int nextPowerOfTwo(unsigned int n) const;
};

#endif /* end of include guard: FFTCONVOLVER_H_DLHBFNNX */
