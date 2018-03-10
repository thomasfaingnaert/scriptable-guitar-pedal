#include <algorithm>
#include <cmath>
#include <iterator>

#include "sinesource.h"

SineSource::SineSource(float amplitude, unsigned int period)
    : amplitude(amplitude), period(period), samples(period)
{
    // pre calculate one period of sine
    unsigned int phase = 0;
    std::generate(samples.begin(), samples.end(), [&phase, amplitude, period] ()
            {
                float res = amplitude * std::sin(2 * M_PI / static_cast<float>(period) * phase);
                phase++;
                return res;
            });

    currentSample = samples.begin();
}

void SineSource::generate_next()
{
    unsigned int numCopied = 0;
    auto block = std::make_shared<std::vector<float>>(BLOCK_SIZE);

    while (numCopied != BLOCK_SIZE)
    {
        if (std::distance(currentSample, samples.end()) >= BLOCK_SIZE - numCopied)
        {
            std::copy_n(currentSample, BLOCK_SIZE - numCopied, block->begin() + numCopied);
            currentSample += BLOCK_SIZE - numCopied;
            numCopied = BLOCK_SIZE;
        }
        else
        {
            std::copy(currentSample, samples.end(), block->begin() + numCopied);
            numCopied += std::distance(currentSample, samples.end());
            currentSample = samples.begin();
        }
    }

    generate(block);
}
