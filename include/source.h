#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

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
            unsigned int channel;
        };

        void connect(const std::shared_ptr<Sink<T>>& sink, unsigned int channel)
        {
            Connection c;
            c.sink = sink;
            c.channel = channel;
            connections.push_back(c);
        }

        void generate(const std::shared_ptr<std::vector<T>>& t)
        {
            if (t->size() != BLOCK_SIZE)
                throw std::invalid_argument("Samples should be BLOCK_SIZE long");

            // update all connected sinks
            for (Connection& c : connections)
            {
                c.sink->push(t, c.channel);
            }
        }

        static constexpr std::size_t BLOCK_SIZE = 256;

    private:
        std::vector<Connection> connections;
};

template<typename T>
constexpr std::size_t Source<T>::BLOCK_SIZE;


#endif /* end of include guard: SOURCE_H_WUKQSVNX */
