#include <cstddef>
#include <vector>

#include "processor.h"

#ifndef ADDER_H_AOFQV6G9
#define ADDER_H_AOFQV6G9

class Adder : public Processor<float, float>
{
    public:
        Adder()
            : Processor<float, float>(2) { }
        virtual std::shared_ptr<std::vector<float>> process(const std::vector<std::shared_ptr<std::vector<float>>>& data) const;
};

#endif /* end of include guard: ADDER_H_AOFQV6G9 */
