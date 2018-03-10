#include <algorithm>
#include <cmath>
#include <iterator>

#include "sinesource.h"

SineSource::SineSource(float amplitude, unsigned int period)
    : amplitude(amplitude), period(period), samples(BLOCK_SIZE + period - 1), currentSample(0)
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
    auto block = std::make_shared<std::vector<float>>(samples.begin() + currentSample, samples.begin() + currentSample + BLOCK_SIZE);
    currentSample = (currentSample + BLOCK_SIZE) % period;
    generate(block);
}
