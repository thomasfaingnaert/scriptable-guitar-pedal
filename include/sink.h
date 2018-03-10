#include <cstddef>

#ifndef SINK_H_FUKZZ1SW
#define SINK_H_FUKZZ1SW

template<typename T>
class Sink
{
    public:
        virtual ~Sink() { };
        virtual void push(const T* t, unsigned int channel) = 0;
};

#endif /* end of include guard: SINK_H_FUKZZ1SW */
