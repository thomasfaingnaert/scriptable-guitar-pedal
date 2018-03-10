#include <algorithm>
#include <iterator>
#include <ostream>

#include "sink.h"
#include "source.h"

#ifndef STREAMSINK_H_WL7BUK4D
#define STREAMSINK_H_WL7BUK4D

template<typename T>
class StreamSink : public Sink<T>
{
    public:
        StreamSink(std::ostream& os) : os(os) { }
        virtual std::size_t getNumChannels() const { return 1; }
        virtual void push(const T* t, unsigned int channel)
        {
            os << "channel " << channel << ": ";
            std::copy(t, t + Source<T>::BLOCK_SIZE, std::ostream_iterator<T>(os, " "));
            os << "\n";
        }

    private:
        std::ostream& os;
};

#endif /* end of include guard: STREAMSINK_H_WL7BUK4D */
