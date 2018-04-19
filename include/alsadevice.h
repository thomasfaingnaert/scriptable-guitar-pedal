#ifndef SCRIPTABLE_GUITAR_PEDAL_ALSADEVICE_H
#define SCRIPTABLE_GUITAR_PEDAL_ALSADEVICE_H

#include "tinyalsa/asoundlib.h"

#include "sink.h"
#include "source.h"

class AlsaDevice : public Sink<float>, public Source<float>
{
public:
    AlsaDevice(unsigned int card,
               unsigned int device,
               unsigned int rate,
               unsigned int channels_in,
               unsigned int channels_out,
               unsigned int period_size_in,
               unsigned int period_size_out,
               unsigned int period_count_in,
               unsigned int period_count_out);

    virtual ~AlsaDevice();

    virtual void push(const std::array<float, Constants::BLOCK_SIZE>& data);

    void generate_next();
private:
    pcm *pcm_in;
    pcm *pcm_out;
};

#endif
