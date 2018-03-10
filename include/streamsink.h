#include <algorithm>
#include <iterator>
#include <memory>
#include <ostream>
#include <vector>

#include "sink.h"
#include "source.h"

#ifndef STREAMSINK_H_WL7BUK4D
#define STREAMSINK_H_WL7BUK4D

template<typename T>
class StreamSink : public Sink<T>
{
    public:
        StreamSink(std::ostream& os) : os(os) { }
        virtual void push(const std::shared_ptr<std::vector<T>>& t, unsigned int channel)
        {
            os << "channel " << channel << ": ";
            std::copy(t->begin(), t->end(), std::ostream_iterator<T>(os, " "));
            os << "\n";
        }

    private:
        std::ostream& os;
};

#endif /* end of include guard: STREAMSINK_H_WL7BUK4D */
