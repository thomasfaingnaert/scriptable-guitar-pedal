#include <iostream>

#include "NE10.h"
#include "adder.h"

std::vector<float> Adder::process(const std::vector<const float*>& data) const
{
    std::vector<float> result(BLOCK_SIZE);
    ne10_add_float(result.data(), const_cast<float*>(data[0]), const_cast<float*>(data[1]), BLOCK_SIZE);
    return result;
}
