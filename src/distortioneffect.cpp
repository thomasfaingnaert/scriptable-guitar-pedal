#include <algorithm>
#include <cmath>

#include "distortioneffect.h"
#include "../include/distortioneffect.h"

DistortionEffect::DistortionEffect(float steepness, float mix)
    : DistortionEffect(steepness, steepness, mix, mix)
{
}

DistortionEffect::DistortionEffect(float positiveSteepness, float negativeSteepness, float positiveMix, float negativeMix)
    : DistortionEffect(positiveSteepness, negativeSteepness, positiveMix, negativeMix, 1e-3f)
{
}

DistortionEffect::DistortionEffect(float positiveSteepness, float negativeSteepness, float positiveMix, float negativeMix, float noiseTreshold)
    : Processor(1), positiveSteepness(positiveSteepness), negativeSteepness(negativeSteepness), positiveMix(positiveMix), negativeMix(negativeMix), noiseTreshold(noiseTreshold)
{
}

std::shared_ptr<std::vector<float>> DistortionEffect::process(const std::vector<std::shared_ptr<std::vector<float>>>& data)
{
    auto result = std::make_shared<std::vector<float>>(BLOCK_SIZE);
    std::transform(data[0]->begin(), data[0]->end(), result->begin(), [this] (float x)
            {
                if (x < -noiseTreshold)
                    return positiveMix * 2.0f / static_cast<float>(M_PI) * std::atan(positiveSteepness * x) + (1 - positiveMix) * x;
                else if (x > noiseTreshold)
                    return negativeMix * 2.0f / static_cast<float>(M_PI) * std::atan(negativeSteepness * x) + (1 - negativeMix) * x;
                else
                    return x;
            });
    return result;
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
