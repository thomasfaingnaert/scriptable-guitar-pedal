#include <iostream>

#include "NE10.h"
#include "adder.h"

std::vector<float> Adder::process(const std::vector<std::vector<float>>& data) const
{
    std::vector<float> result(blockSize);
    ne10_add_float(result.data(), const_cast<float*>(data[0].data()), const_cast<float*>(data[1].data()), blockSize);
    return result;
}
