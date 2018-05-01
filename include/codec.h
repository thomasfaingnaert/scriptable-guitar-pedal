#ifndef SCRIPTABLE_GUITAR_PEDAL_CODEC_H
#define SCRIPTABLE_GUITAR_PEDAL_CODEC_H

#include "i2cdevice.h"

class Codec : I2CDevice
{
public:
    Codec(unsigned int adapter, std::uint16_t deviceAddress);

    enum FunctionalMode : std::uint8_t
    {
        SINGLE_SPEED_MODE_1 = 0b00,
        SINGLE_SPEED_MODE_2 = 0b01,
        DOUBLE_SPEED_MODE = 0b10,
        QUAD_SPEED_MODE = 0b11
    };

    static constexpr std::uint8_t functionalModeOffset = 6;

    void setFunctionalMode(FunctionalMode functionalMode)
    {
        writeRegister(MODE_CONTROL_1, functionalMode << functionalModeOffset);
    };

    enum ClockingRatio : std::uint8_t
    {
        CLOCKING_RATIO_1 = 0b00,
        CLOCKING_RATIO_2 = 0b01,
        CLOCKING_RATIO_3 = 0b10,
        CLOCKING_RATIO_4 = 0b11
    };

    static constexpr std::uint8_t clockingRatioOffset = 4;

    void setClockingRatio(ClockingRatio clockingRatio)
    {
        writeRegister(MODE_CONTROL_1, clockingRatio << clockingRatioOffset);
    }

    enum MasterSlaveMode : std::uint8_t
    {
        SLAVE = 0b0,
        MASTER = 0b1
    };

    static constexpr std::uint8_t masterSlaveModeOffset = 3;

    void setMasterSlaveMode(MasterSlaveMode masterSlaveMode)
    {
        writeRegister(MODE_CONTROL_1, masterSlaveMode << masterSlaveModeOffset);
    }

    enum DigitalInterfaceFormat : std::uint8_t
    {
        LEFT_JUSTIFIED = 0b000,
        INTER_IC_SOUND = 0b001,
        RIGHT_JUSTIFIED_16 = 0b010,
        RIGHT_JUSTIFIED_18 = 0b101,
        RIGHT_JUSTIFIED_20 = 0b100,
        RIGHT_JUSTIFIED_24 = 0b011
    };

    static constexpr std::uint8_t digitalInterfaceFormatOffset = 0;

    void setDACDigitalInterfaceFormat(DigitalInterfaceFormat digitalInterfaceFormat)
    {
        writeRegister(MODE_CONTROL_1, digitalInterfaceFormat << digitalInterfaceFormatOffset);
    }

private:
    enum Register : std::uint8_t
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
