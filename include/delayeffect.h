#include <deque>
#include <memory>
#include <stdexcept>
#include <vector>

#include "processor.h"

#ifndef DELAYEFFECT_H_Q0QOLA9M
#define DELAYEFFECT_H_Q0QOLA9M

class DelayEffect : public Processor<float, float>
{
    public:
        DelayEffect(float mainCoeff, const std::vector<unsigned int>& delays, const std::vector<float>& coeffs);
        virtual std::shared_ptr<std::vector<float>> process(const std::vector<std::shared_ptr<std::vector<float>>>& data);

    private:
        float mainCoeff;
        std::vector<unsigned int> delays; // in blocks!
        std::vector<float> coeffs;
        std::deque<std::shared_ptr<std::vector<float>>> buffer;
};

#endif /* end of include guard: DELAYEFFECT_H_Q0QOLA9M */
