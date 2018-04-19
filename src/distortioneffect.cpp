#include <algorithm>
#include <cmath>
#include <iostream>

#include "distortioneffect.h"

DistortionEffect::DistortionEffect(float steepness, float mix)
    : DistortionEffect(steepness, steepness, mix, mix)
{
}

DistortionEffect::DistortionEffect(float positiveSteepness, float negativeSteepness, float positiveMix, float negativeMix)
    : DistortionEffect(positiveSteepness, negativeSteepness, positiveMix, negativeMix, 1e-3f)
{
}

DistortionEffect::DistortionEffect(float positiveSteepness, float negativeSteepness, float positiveMix, float negativeMix, float noiseTreshold)
    : positiveSteepness(positiveSteepness), negativeSteepness(negativeSteepness), positiveMix(positiveMix), negativeMix(negativeMix), noiseTreshold(noiseTreshold)
{
}

void DistortionEffect::push(const std::array<float, Constants::BLOCK_SIZE>& data)
{
    std::array<float, Constants::BLOCK_SIZE> result;
    std::transform(data.begin(), data.end(), result.begin(), [this] (float x) -> float
            {
                if (x < -noiseTreshold)
                    return positiveMix * 2.0f / static_cast<float>(M_PI) * std::atan(positiveSteepness * x) + (1 - positiveMix) * x;
                else if (x > noiseTreshold)
                    return negativeMix * 2.0f / static_cast<float>(M_PI) * std::atan(negativeSteepness * x) + (1 - negativeMix) * x;
                else
                    return x;
            });
    generate(result);
}

void DistortionEffect::setPositiveGain(float positiveSteepness)
{
    DistortionEffect::positiveSteepness = positiveSteepness;
}

void DistortionEffect::setNegativeGain(float negativeSteepness)
{
    DistortionEffect::negativeSteepness = negativeSteepness;
}

void DistortionEffect::setPositiveMix(float positiveMix)
{
    DistortionEffect::positiveMix = positiveMix;
}

void DistortionEffect::setNegativeMix(float negativeMix)
{
    DistortionEffect::negativeMix = negativeMix;
}

void DistortionEffect::setNoiseTreshold(float noiseTreshold)
{
    DistortionEffect::noiseTreshold = noiseTreshold;
}
