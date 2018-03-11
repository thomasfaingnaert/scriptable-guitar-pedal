#include <memory>
#include <vector>

#include "processor.h"

#ifndef TREMOLOEFFECT_H_2DI4OLXP
#define TREMOLOEFFECT_H_2DI4OLXP

class TremoloEffect : public Processor<float, float>
{
    public:
        TremoloEffect(float depth, unsigned int period);
        virtual std::shared_ptr<std::vector<float>> process(const std::vector<std::shared_ptr<std::vector<float>>>& data);

    private:
        float depth;
        unsigned int period;
        std::vector<float> coeffs;
        unsigned int currentSample;
};

#endif /* end of include guard: TREMOLOEFFECT_H_2DI4OLXP */

