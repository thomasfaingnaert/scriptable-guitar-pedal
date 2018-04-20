#ifndef FILTEREFFECT_H_MXLN2IIZ
#define FILTEREFFECT_H_MXLN2IIZ

#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "fftconvolver.h"
#include "source.h"
#include "sink.h"

class FilterEffect : public Source<float>, public Sink<float>
{
    public:
        FilterEffect(const std::vector<float>& impulseResponse);
        virtual void push(const std::array<float, Constants::BLOCK_SIZE>& data);

    private:
        class MiniConvolver // convolver for only part of impulse response
        {
            public:
                MiniConvolver(const std::vector<float>& impulseResponse, unsigned int delay, bool inBackground);
                unsigned int getBlockSize() const { return blockSize; }
                void calculate(const std::vector<float>& input);
                std::vector<float> getNextBlock();

            private:
                FFTConvolver conv;
                unsigned int blockSize;
                std::deque<float> outputBuffer;
                bool inBackground;

                std::thread thread;
                std::unique_ptr<std::mutex> mutex;
                std::unique_ptr<std::condition_variable> cond_var;
        };

        std::vector<MiniConvolver> convolvers; // each convolver is responsible for one part of impulse response
        std::vector<float> inputBuffer; // used to accumulate enough input samples for the convolvers
        unsigned int numBlocksReceived; // used to remember how many blocks have arrived
};

#endif /* end of include guard: FILTEREFFECT_H_MXLN2IIZ */
