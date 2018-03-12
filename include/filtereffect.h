#include <cmath>
#include <vector>

#include "dspmath.h"
#include "sampledata.h"
#include "NE10.h"

#ifndef FILTEREFFECT_H_DQGPDEBX
#define FILTEREFFECT_H_DQGPDEBX

class FilterEffect
{
    public:
        ~FilterEffect();
        void setImpulseResponse(const std::vector<Sample>& impulseResponse);
        std::vector<Complex> getFrequencyResponse() const;
        unsigned int getBlockSize() const;
        void addInputBlock(const std::vector<Sample>& block);
        std::vector<Sample> getOutputBlock();

    private:
        ne10_fft_r2c_cfg_float32_t config = 0;

        unsigned int overlap;
        unsigned int period;
        unsigned int blockSize;

        std::vector<Complex> frequencyResponse;
        std::vector<Sample> inputBuffer;

        unsigned int nextPowerOfTwo(unsigned int n) const;
};

#endif /* end of include guard: FILTEREFFECT_H_DQGPDEBX */
