#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

#include "samplebuffer.h"
#include "sink.h"

#ifndef SOURCE_H_WUKQSVNX
#define SOURCE_H_WUKQSVNX

template<typename T>
class Source
{
    public:
        struct Connection
        {
            std::shared_ptr<Sink<T>> sink;
            int channel;
            int counter;
        };

        void connect(const std::shared_ptr<Sink<T>>& sink, int channel)
        {
            Connection c;
            c.sink = sink;
            c.channel = channel;
            c.counter = 0;
            connections.push_back(c);
        }

        void generate(const T& t)
        {
            buffer.push_back(t);

            // update all connected sinks
            for (Connection& c : connections)
            {
                c.counter++;

                if (c.counter == BLOCK_SIZE)
                {
                    c.counter = 0;
                    c.sink->push(buffer.data(), c.channel);
                }
            }
        }

        static constexpr std::size_t BLOCK_SIZE = 256;
        Source() : buffer(BLOCK_SIZE) { }

    private:
        SampleBuffer<T> buffer;
        std::vector<Connection> connections;
};

#endif /* end of include guard: SOURCE_H_WUKQSVNX */
