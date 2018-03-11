#include <memory>
#include <stdexcept>
#include <vector>

#include "NE10.h"
#include "dspmath.h"

#ifndef CONVOLVER_H_DICGZJT1
#define CONVOLVER_H_DICGZJT1

class Convolver
{
    public:
        Convolver(const std::vector<float>& impulseResponse, unsigned int blockSize);
        ~Convolver();

        std::shared_ptr<std::vector<float>> process(const std::shared_ptr<std::vector<float>>& block);

    private:
        std::vector<Complex> frequencyResponse;
        unsigned int overlap;
        unsigned int blockSize;
        unsigned int period;
        ne10_fft_r2c_cfg_float32_t config;
        std::shared_ptr<std::vector<float>> lastBlock;

        unsigned int nextPowerOfTwo(unsigned int n) const;
};

#endif /* end of include guard: CONVOLVER_H_DICGZJT1 */
