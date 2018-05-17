#ifndef SCRIPTABLE_GUITAR_PEDAL_CODEC_H
#define SCRIPTABLE_GUITAR_PEDAL_CODEC_H

#include "i2cdevice.h"

class Codec : I2CDevice
{
public:
    Codec(unsigned int adapter, uint16_t deviceAddress);

    enum Register : uint8_t
    {
        MODE_CONTROL_1 = 0x01,
        DAC_CONTROL = 0x02,
        DAC_VOLUME_AND_MIXING_CONTROL = 0x03,
        DAC_CHANNEL_A_VOLUME_CONTROL = 0x04,
        DAC_CHANNEL_B_VOLUME_CONTROL = 0x05,
        ADC_CONTROL = 0x06,
        MODE_CONTROL_2 = 0x07,
        CHIP_ID = 0x08
    };
};

#endif
