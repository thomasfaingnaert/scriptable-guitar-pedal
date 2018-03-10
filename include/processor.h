#include <cstddef>
#include <deque>
#include <vector>

#include "sink.h"
#include "source.h"

#ifndef PROCESSOR_H_DGNQSWPH
#define PROCESSOR_H_DGNQSWPH

template<typename T, typename U>
class Processor : public Source<T>, public Sink<U>
{
    public:
        Processor(std::size_t blockSize, std::size_t numChannels)
            : data(numChannels), channelsReceived(0), blockSize(blockSize), numChannels(numChannels)
        { }

        virtual std::size_t getBlockSize(unsigned int channel) const { return blockSize; }

        virtual void push(const U* u, std::size_t n, unsigned int channel)
        {
            if (channel >= numChannels)
                return;

            // save data
            if (data[channel].empty())
                ++channelsReceived;
            data[channel].push_back(std::vector<U>(u, u+n));

            if (channelsReceived == numChannels)
            {
                channelsReceived = 0;

                std::vector<std::vector<U>> samples;
                for (std::deque<std::vector<U>>& channelData : data)
                {
                    samples.push_back(channelData.front());
                    channelData.pop_front();

                    if (!channelData.empty())
                        ++channelsReceived;
                }

                std::vector<T> output = process(samples);
                for (T t : output)
                {
                    Source<T>::generate(t);
                }
            }
        }

        virtual std::vector<T> process(const std::vector<std::vector<U>>& data) const = 0;

    protected:
        std::size_t blockSize;

    private:
        std::vector<std::deque<std::vector<U>>> data;
        std::size_t numChannels;
        std::size_t channelsReceived;
};

#endif /* end of include guard: PROCESSOR_H_DGNQSWPH */
