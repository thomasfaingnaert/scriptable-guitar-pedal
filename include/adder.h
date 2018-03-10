#include <cstddef>
#include <vector>

#include "processor.h"

#ifndef ADDER_H_AOFQV6G9
#define ADDER_H_AOFQV6G9

class Adder : public Processor<float, float>
{
    public:
        Adder(std::size_t blockSize)
            : Processor<float, float>(blockSize, 2) { }
        virtual std::vector<float> process(const std::vector<const float*>& data) const;
};

#endif /* end of include guard: ADDER_H_AOFQV6G9 */
