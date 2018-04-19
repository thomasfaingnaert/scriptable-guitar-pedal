#ifndef TREMOLOEFFECT_H_2DI4OLXP
#define TREMOLOEFFECT_H_2DI4OLXP

#include "sink.h"
#include "source.h"

class TremoloEffect : public Source<float>, public Sink<float>
{
    public:
        TremoloEffect(float depth, unsigned int period);
        virtual void push(const std::array<float, Constants::BLOCK_SIZE>& data);

    private:
        float depth;
        unsigned int period;
        std::vector<float> coeffs;
        unsigned int currentSample;
};

#endif /* end of include guard: TREMOLOEFFECT_H_2DI4OLXP */

