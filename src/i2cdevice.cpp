#include "i2cdevice.h"

#include <string>

#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <sys/ioctl.h>


I2CDevice::I2CDevice(unsigned int adapter, uint16_t deviceAddress) : deviceAddress(deviceAddress)
{
    std::string filename = "/dev/i2c-" + std::to_string(adapter);

    file = open(filename.c_str(), O_RDWR);

    if (file < 0)
    {

    }

    if (ioctl(file, I2C_FUNCS, &functionality) < 0)
    {

    }
}

I2CDevice::~I2CDevice()
{
    close(file);
}

void I2CDevice::writeRegister(uint16_t registerAddress, uint8_t *sendBuffer, uint16_t sendBufferSize) const
{

}

void I2CDevice::readRegister(uint8_t registerAddress, uint8_t *receiveBuffer, uint16_t receiveBufferSize) const
{

}
