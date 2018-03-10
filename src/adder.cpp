#include <iostream>

#include "NE10.h"
#include "adder.h"

std::shared_ptr<std::vector<float>> Adder::process(const std::vector<std::shared_ptr<std::vector<float>>>& data) const
{
    std::shared_ptr<std::vector<float>> result = std::make_shared<std::vector<float>>(BLOCK_SIZE);
    ne10_add_float(result->data(), const_cast<float*>(data[0]->data()), const_cast<float*>(data[1]->data()), BLOCK_SIZE);
    return result;
}
