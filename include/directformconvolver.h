#ifndef DIRECTFORMCONVOLVER_H_NGLIOZHS
#define DIRECTFORMCONVOLVER_H_NGLIOZHS

#include "NE10.h"
#include <vector>

class DirectFormConvolver
{
    public:
        DirectFormConvolver(const std::vector<float>& impulseResponse, unsigned int blockSize);
        std::vector<float> process(const std::vector<float>& input);

    private:
        unsigned int blockSize;
        ne10_fir_instance_f32_t config;
        std::vector<float> state;
        std::vector<float> taps;
};

#endif /* end of include guard: DIRECTFORMCONVOLVER_H_NGLIOZHS */
