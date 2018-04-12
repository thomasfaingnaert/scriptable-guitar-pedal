#ifndef SCRIPTABLE_GUITAR_PEDAL_ALSASINK_H
#define SCRIPTABLE_GUITAR_PEDAL_ALSASINK_H

#include "sink.h"

class AlsaSink : public Sink<float>
{
    public:
        AlsaSink(unsigned int card, unsigned int device, unsigned int sampling_frequency);

        virtual void push(const std::shared_ptr<std::vector<float>> &t, unsigned int channel);

    private:
        struct pcm *pcm;
};

#endif
