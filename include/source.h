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

        Source() : buffer(MAX_BLOCK_SIZE) { }

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

                const std::size_t blockSize = c.sink->getBlockSize(c.channel);
                if (blockSize > MAX_BLOCK_SIZE) throw std::invalid_argument("Blocksize cannot be bigger than MAX_BLOCK_SIZE");
                if (c.counter == blockSize)
                {
                    c.counter = 0;
                    // buffer.data() points to the last MAX_BLOCK_SIZE samples
                    // the last blockSize samples start at index buffer.data() + MAX_BLOCKS_SIZE - blockSize
                    c.sink->push(buffer.data() + MAX_BLOCK_SIZE - blockSize, blockSize, c.channel);
                }
            }
        }

    private:
        static constexpr std::size_t MAX_BLOCK_SIZE = 256;
        SampleBuffer<T> buffer;
        std::vector<Connection> connections;
};

#endif /* end of include guard: SOURCE_H_WUKQSVNX */
