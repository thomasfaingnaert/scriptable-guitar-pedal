#ifndef FILTEREFFECT_H_MXLN2IIZ
#define FILTEREFFECT_H_MXLN2IIZ

#include "fftconvolver.h"
#include "processor.h"

class FilterEffect : public Processor<float, float>
{
    public:
        FilterEffect();
        virtual std::shared_ptr<std::vector<float>> process(const std::vector<std::shared_ptr<std::vector<float>>> &data);

    private:
        class MiniConvolver // convolver for only part of impulse response
        {
            public:
                MiniConvolver(const std::vector<float>& impulseResponse);
                unsigned int getBlockSize() const { return blockSize; }
                void calculate(const std::vector<float>& input);
                std::vector<float> getPreviousResult() const { return prevResult; }
                std::vector<float> getLastResult() const { return result; }

                unsigned int index;

            private:
                FFTConvolver conv;
                unsigned int blockSize;
                std::vector<float> result;
                std::vector<float> prevResult;
        };

        std::vector<MiniConvolver> convolvers; // each convolver is responsible for one part of impulse response
        std::vector<float> inputBuffer; // used to accumulate enough input samples for the convolvers
        unsigned int numBlocksReceived; // used to remember how many blocks have arrived
};

#endif /* end of include guard: FILTEREFFECT_H_MXLN2IIZ */
