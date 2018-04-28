#ifndef FILTEREFFECT_H_TJL7ARNQ
#define FILTEREFFECT_H_TJL7ARNQ

#include "sink.h"
#include "source.h"

class FilterEffect : public Source<float>, public Sink<float>
{
    public:
        virtual void push(const std::array<float, Constants::BLOCK_SIZE>& data);
};

#endif /* end of include guard: FILTEREFFECT_H_TJL7ARNQ */
