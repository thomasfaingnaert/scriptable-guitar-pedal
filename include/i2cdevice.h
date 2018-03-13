#ifndef SCRIPTABLE_GUITAR_PEDAL_I2C_H
#define SCRIPTABLE_GUITAR_PEDAL_I2C_H

#include <cstdint>


class I2CDevice
{
public:
    I2CDevice(unsigned int adapter, uint16_t deviceAddress);

    virtual ~I2CDevice();

    void writeRegister(uint16_t registerAddress, uint8_t *sendBuffer, uint16_t sendBufferSize) const;

    void readRegister(uint8_t registerAddress, uint8_t *receiveBuffer, uint16_t receiveBufferSize) const;
private:
    int file;

    unsigned long functionality;

    uint16_t deviceAddress;
};

#endif
