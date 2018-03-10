#include <cstddef>
#include <vector>

#include "sink.h"
#include "source.h"

#ifndef PROCESSOR_H_DGNQSWPH
#define PROCESSOR_H_DGNQSWPH

template<typename T, typename U>
class Processor : public Source<T>, public Sink<U>
{
    public:
        Processor(std::size_t numChannels)
            : data(numChannels), channelsReceived(0), numChannels(numChannels)
        { }

        virtual void push(const U* u, unsigned int channel)
        {
            if (channel >= numChannels)
                return;

            // save pointer
            data[channel] = u;

            // one more channel received
            ++channelsReceived;

            // check if all channels are received
            if (channelsReceived == numChannels)
            {
                channelsReceived = 0;
                std::vector<T> output = process(data);
                for (T t : output)
                {
                    Source<T>::generate(t);
                }
            }
        }

        virtual std::vector<T> process(const std::vector<const U*>& data) const = 0;

    private:
        std::vector<const U*> data; // used to remember the address of each input
        std::size_t channelsReceived;
        std::size_t numChannels;
};

#endif /* end of include guard: PROCESSOR_H_DGNQSWPH */
