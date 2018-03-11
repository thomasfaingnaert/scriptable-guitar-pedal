#include <memory>
#include <vector>

#include "processor.h"

#ifndef DISTORTIONEFFECT_H_1UL2TEIH
#define DISTORTIONEFFECT_H_1UL2TEIH

class DistortionEffect : public Processor<float, float>
{
    public:
        DistortionEffect(float steepness, float mix);
        DistortionEffect(float positiveSteepness, float negativeSteepness, float positiveMix, float negativeMix);
        DistortionEffect(float positiveSteepness, float negativeSteepness, float positiveMix, float negativeMix, float noiseTreshold);
        virtual std::shared_ptr<std::vector<float>> process(const std::vector<std::shared_ptr<std::vector<float>>>& data);

    private:
        float positiveSteepness;
        float negativeSteepness;
        float positiveMix;
        float negativeMix;
        float noiseTreshold;
};

#endif /* end of include guard: DISTORTIONEFFECT_H_1UL2TEIH */
