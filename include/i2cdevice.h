#ifndef SCRIPTABLE_GUITAR_PEDAL_I2C_H
#define SCRIPTABLE_GUITAR_PEDAL_I2C_H

#include <cstdint>


class I2CDevice
{
public:
    I2CDevice(unsigned int adapter, std::uint16_t deviceAddress);

    virtual ~I2CDevice();

protected:
    void writeRegister(std::uint8_t registerAddress, std::uint8_t value) const;

    std::uint8_t readRegister(std::uint8_t registerAddress) const;

    void updateRegister(std::uint8_t registerAddress, std::uint8_t mask, std::uint8_t value) const;

private:
    int file;

    unsigned long functionality;

    const std::uint16_t deviceAddress;
};

#endif
