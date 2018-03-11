#include <algorithm>
#include <cmath>

#include "NE10.h"
#include "tremoloeffect.h"

TremoloEffect::TremoloEffect(float depth, unsigned int period)
    : Processor(1), depth(depth), period(period), coeffs(BLOCK_SIZE + period - 1), currentSample(0)
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

std::shared_ptr<std::vector<float>> TremoloEffect::process(const std::vector<std::shared_ptr<std::vector<float>>>& data)
{
    auto result = std::make_shared<std::vector<float>>(BLOCK_SIZE);
    ne10_mul_float(result->data(), const_cast<float*>(data[0]->data()), const_cast<float*>(coeffs.data() + currentSample), BLOCK_SIZE);
    currentSample = (currentSample + BLOCK_SIZE) % period;
    return result;
}

