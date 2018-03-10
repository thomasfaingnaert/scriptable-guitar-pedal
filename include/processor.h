#include <cstddef>
#include <stdexcept>
#include <vector>

#include "sink.h"
#include "source.h"

#ifndef PROCESSOR_H_DGNQSWPH
#define PROCESSOR_H_DGNQSWPH

template<typename T, typename U>
class Processor : public Source<T>, public Sink<U>
{
    public:
        Processor(unsigned int numChannels)
            : data(numChannels), numChannels(numChannels), channelsReceived(0)
        { }

        virtual void push(const std::shared_ptr<std::vector<U>>& u, unsigned int channel)
        {
            if (channel >= numChannels)
                throw std::invalid_argument("Channel cannot be bigger than numChannels");

            // save data
            data[channel] = u;

            // one more channel received
            ++channelsReceived;

            // check if all channels are received
            if (channelsReceived == numChannels)
            {
                channelsReceived = 0;
                std::shared_ptr<std::vector<T>> output = process(data);
                Source<T>::generate(output);
            }
        }

        virtual std::shared_ptr<std::vector<T>> process(const std::vector<std::shared_ptr<std::vector<U>>>& data) const = 0;

    private:
        std::vector<std::shared_ptr<std::vector<U>>> data;
        unsigned int numChannels;
        unsigned int channelsReceived;
};

#endif /* end of include guard: PROCESSOR_H_DGNQSWPH */
