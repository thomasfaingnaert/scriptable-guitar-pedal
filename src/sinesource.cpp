#include <algorithm>
#include <cmath>
#include <iterator>

#include "constants.h"
#include "sinesource.h"

SineSource::SineSource(float amplitude, unsigned int period)
    : amplitude(amplitude), period(period), samples(Constants::BLOCK_SIZE + period - 1), currentSample(0)
{
    // pre calculate sine
    unsigned int phase = 0;
    std::generate(samples.begin(), samples.end(), [&phase, amplitude, period] ()
            {
                float res = amplitude * std::sin(2 * M_PI / static_cast<float>(period) * phase);
                phase++;
                return res;
            });
}

void SineSource::generate_next()
{
    std::array<float, Constants::BLOCK_SIZE> block;
    std::copy_n(samples.begin() + currentSample, Constants::BLOCK_SIZE, block.begin());
    currentSample = (currentSample + Constants::BLOCK_SIZE) % period;
    generate(block);
}

