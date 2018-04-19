#include <algorithm>
#include <cmath>

#include "constants.h"
#include "NE10.h"

#include "tremoloeffect.h"

TremoloEffect::TremoloEffect(float depth, unsigned int period)
    : depth(depth), period(period), coeffs(Constants::BLOCK_SIZE + period - 1), currentSample(0)
{
    // pre calculate coeffs
    unsigned int phase = 0;
    std::generate(coeffs.begin(), coeffs.end(), [&phase, depth, period] ()
            {
                float res = (1 - depth) + depth * std::sin(2 * M_PI / static_cast<float>(period) * phase);
                phase++;
                return res;
            });
}

void TremoloEffect::push(const std::array<float, Constants::BLOCK_SIZE>& data)
{
    std::array<float, Constants::BLOCK_SIZE> result;
    ne10_mul_float(result.data(), const_cast<float*>(data.data()), coeffs.data() + currentSample, Constants::BLOCK_SIZE);
    currentSample = (currentSample + Constants::BLOCK_SIZE) % period;
    generate(result);
}

