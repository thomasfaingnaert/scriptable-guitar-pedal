#ifndef DISTORTIONEFFECT_H_1UL2TEIH
#define DISTORTIONEFFECT_H_1UL2TEIH

#include <vector>

#include "source.h"
#include "sink.h"

class DistortionEffect : public Source<float>, public Sink<float>
{
public:
    DistortionEffect(float steepness, float mix);
    DistortionEffect(float positiveSteepness, float negativeSteepness, float positiveMix, float negativeMix);
    DistortionEffect(float positiveSteepness, float negativeSteepness, float positiveMix, float negativeMix, float noiseTreshold);
    virtual void push(const std::array<float, Constants::BLOCK_SIZE>& data);
    void setPositiveGain(float positiveSteepness);
    void setNegativeGain(float negativeSteepness);
    void setPositiveMix(float positiveMix);
    void setNegativeMix(float negativeMix);
    void setNoiseTreshold(float noiseTreshold);

private:
    float positiveSteepness;
    float negativeSteepness;
    float positiveMix;
    float negativeMix;
    float noiseTreshold;
};

#endif /* end of include guard: DISTORTIONEFFECT_H_1UL2TEIH */
