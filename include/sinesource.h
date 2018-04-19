#ifndef SINESOURCE_H_VSRHKGEL
#define SINESOURCE_H_VSRHKGEL

#include <array>
#include <vector>

#include "source.h"

class SineSource : public Source<float>
{
    public:
        SineSource(float amplitude, unsigned int period);
        void generate_next();

    private:
        float amplitude;
        unsigned int period;
        std::vector<float> samples;
        unsigned int currentSample;
};

#endif /* end of include guard: SINESOURCE_H_VSRHKGEL */
