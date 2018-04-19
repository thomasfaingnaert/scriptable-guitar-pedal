#ifndef SINK_H_D2GUWTG5
#define SINK_H_D2GUWTG5

#include <array>

#include "constants.h"
#include "source.h"

template<typename T>
class Sink
{
    public:
        virtual ~Sink() { }
        virtual void push(const std::array<T, Constants::BLOCK_SIZE>& data) = 0;
};

#endif /* end of include guard: SINK_H_D2GUWTG5 */
