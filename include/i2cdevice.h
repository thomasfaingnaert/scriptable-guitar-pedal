#ifndef SCRIPTABLE_GUITAR_PEDAL_I2C_H
#define SCRIPTABLE_GUITAR_PEDAL_I2C_H

#include <cstdint>

class I2CDevice
{
public:
    I2CDevice(unsigned int adapter, uint16_t deviceAddress);

    virtual ~I2CDevice();

    void writeRegister(uint8_t registerAddress, uint8_t value) const;

    uint8_t readRegister(uint8_t registerAddress) const;
private:
    int file;

    unsigned long functionality;

    const uint16_t deviceAddress;
};

#endif
