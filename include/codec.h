#ifndef SCRIPTABLE_GUITAR_PEDAL_CODEC_H
#define SCRIPTABLE_GUITAR_PEDAL_CODEC_H

#include <string>


class Codec
{
public:
    Codec(unsigned int adapter, bool chipAddressSuffix);

    virtual ~Codec();
private:
    static constexpr int CHIP_ADDRESS_PREFIX = 0b001000;

    int file;

    enum Register : uint8_t
    {
        MODE_CONTROL = 0x01,
        DAC_CONTROL = 0x02,
        DAC_VOLUME_MIXING_CONTROL = 0x03,
        DAC_CHANNEL_A_VOLUME_CONTROL = 0x04,
        DAC_CHANNEL_B_VOLUME_CONTROL = 0x05,
        ADC_CONTROL = 0x06,
        MODE_CONTROL_2 = 0x07,
        CHIP_ID = 0x08
    };

    void writeRegister(Register reg, uint8_t value) const;

    uint8_t readRegister(Register reg) const;

    void updateRegister(Register reg, uint8_t mask, uint8_t value) const;
};


#endif
