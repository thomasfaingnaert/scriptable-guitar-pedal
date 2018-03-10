#include "source.h"

#ifndef SINESOURCE_H_1Z38J52N
#define SINESOURCE_H_1Z38J52N

class SineSource : public Source<float>
{
    public:
        SineSource(float amplitude, unsigned int period);
        void generate_next();

    private:
        float amplitude;
        unsigned int period;
        std::vector<float> samples; // pre calculate one period
        std::vector<float>::iterator currentSample;
};

#endif /* end of include guard: SINESOURCE_H_1Z38J52N */
