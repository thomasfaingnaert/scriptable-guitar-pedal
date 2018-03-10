#include <cstddef>

#ifndef SINK_H_FUKZZ1SW
#define SINK_H_FUKZZ1SW

template<typename T>
class Sink
{
    public:
        virtual ~Sink() { };
        virtual std::size_t getBlockSize(int channel) const = 0;
        virtual void push(const T* t, std::size_t n, int channel) const = 0;
};

#endif /* end of include guard: SINK_H_FUKZZ1SW */
