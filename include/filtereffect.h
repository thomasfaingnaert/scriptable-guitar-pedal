#include <cmath>
#include <vector>

#include "sampledata.h"
#include "NE10.h"

#ifndef FILTEREFFECT_H_DQGPDEBX
#define FILTEREFFECT_H_DQGPDEBX

typedef ne10_fft_cpx_float32_t Complex;

class FilterEffect
{
    public:
        void setImpulseResponse(std::vector<Sample> impulseResponse);
        void setFrequencyResponse(const std::vector<Complex>& frequencyResponse);
        std::vector<Complex> getFrequencyResponse() const;

    private:
        ne10_fft_r2c_cfg_float32_t config;
        std::vector<Complex> frequencyResponse;

        unsigned int nextPowerOfTwo(unsigned int n) const;
};

#endif /* end of include guard: FILTEREFFECT_H_DQGPDEBX */
