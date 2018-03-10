#include <algorithm>
#include <iterator>
#include <ostream>

#include "sink.h"

#ifndef STREAMSINK_H_WL7BUK4D
#define STREAMSINK_H_WL7BUK4D

template<typename T>
class StreamSink : public Sink<T>
{
    public:
        StreamSink(std::ostream& os, std::size_t blockSize) : os(os), blockSize(blockSize) { }
        virtual std::size_t getBlockSize(int channel) const { return blockSize; }
        virtual void push(const T* t, std::size_t n, int channel) const
        {
            os << "channel " << channel << ": ";
            std::copy(t, t+n, std::ostream_iterator<T>(os, " "));
            os << "\n";
        }

    private:
        std::ostream& os;
        std::size_t blockSize;
};

#endif /* end of include guard: STREAMSINK_H_WL7BUK4D */
