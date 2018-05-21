#ifndef SOURCE_H_TFQHODIC
#define SOURCE_H_TFQHODIC

#include <array>
#include <memory>
#include <vector>

#include "constants.h"
#include "sink.h"

template<typename T>
class Sink;

template<typename T>
class Source
{
    public:
        void connect(const std::shared_ptr<Sink<T>>& sink)
        {
            connectedSinks.clear();
            connectedSinks.push_back(sink);
        }

        void generate(const std::array<T, Constants::BLOCK_SIZE>& data)
        {
            for (const std::shared_ptr<Sink<T>>& sink : connectedSinks)
            {
                sink->push(data);
            }
        }

    private:
        std::vector<std::shared_ptr<Sink<T>>> connectedSinks;
};

#endif /* end of include guard: SOURCE_H_TFQHODIC */
