#ifndef DELAYEFFECT_H_Q0QOLA9M
#define DELAYEFFECT_H_Q0QOLA9M

#include <deque>
#include <memory>
#include <stdexcept>
#include <vector>

#include "blockbuffer.h"
#include "source.h"
#include "sink.h"

class DelayEffect : public Source<float>, public Sink<float>
{
    public:
        DelayEffect(float mainCoeff, const std::vector<unsigned int>& delays, const std::vector<float>& coeffs);
        virtual void push(const std::array<float, Constants::BLOCK_SIZE>& data);

    private:
        float mainCoeff;
        std::vector<unsigned int> delays;
        std::vector<float> coeffs;
	BlockBuffer<float> inputBuffer;
};

#endif /* end of include guard: DELAYEFFECT_H_Q0QOLA9M */
