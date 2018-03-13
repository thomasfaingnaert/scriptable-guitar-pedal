#include "codec.h"

Codec::Codec(unsigned int adapter, uint16_t deviceAddress) : I2CDevice(adapter, deviceAddress)
{

}

uint8_t Codec::getModeControl1()
{
    return readRegister(MODE_CONTROL_1);
}

uint8_t Codec::getModeControl2()
{
    return readRegister(MODE_CONTROL_2);
}

uint8_t Codec::getDACControl()
{
    return readRegister(DAC_CONTROL);
}

uint8_t Codec::getDACVolumeAndMixingControl()
{
    return readRegister(DAC_VOLUME_AND_MIXING_CONTROL);
}

uint8_t Codec::getDACChannelAVolumeControl()
{
    return readRegister(DAC_CHANNEL_A_VOLUME_CONTROL);
}

uint8_t Codec::getDACChannelBVolumeControl()
{
    return readRegister(DAC_CHANNEL_B_VOLUME_CONTROL);
}

uint8_t Codec::getADCControl()
{
    return readRegister(ADC_CONTROL);
}

uint8_t Codec::getChipID()
{
    return readRegister(CHIP_ID);
}
