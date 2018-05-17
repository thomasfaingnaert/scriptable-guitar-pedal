#include "i2cdevice.h"

#include <string>

#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

I2CDevice::I2CDevice(unsigned int adapter, std::uint16_t deviceAddress) : deviceAddress(deviceAddress)
{
    std::string filename = "/dev/i2c-" + std::to_string(adapter);

    file = open(filename.c_str(), O_RDWR);

    if (file < 0)
    {

    }

    functionality = 0;

    if (ioctl(file, I2C_FUNCS, &functionality) < 0)
    {

    }
}

I2CDevice::~I2CDevice()
{
    close(file);
}

void I2CDevice::writeRegister(std::uint8_t registerAddress, std::uint8_t value) const
{
    std::uint8_t sendBuffer[] = {registerAddress, value};

    i2c_msg messages[1];

    messages[0].addr = deviceAddress;
    messages[0].flags = 0;
    messages[0].len = 2;
    messages[0].buf = sendBuffer;

    i2c_rdwr_ioctl_data data = {};

    data.msgs = messages;
    data.nmsgs = 1;

    if (ioctl(file, I2C_RDWR, &data) < 0)
    {

    }
}

std::uint8_t I2CDevice::readRegister(std::uint8_t registerAddress) const
{
    std::uint8_t sendBuffer[] = {registerAddress};
    std::uint8_t receiveBuffer[] = {0};

    i2c_msg messages[2];

    messages[0].addr = deviceAddress;
    messages[0].flags = 0;
    messages[0].len = 1;
    messages[0].buf = sendBuffer;

    messages[1].addr = deviceAddress;
    messages[1].flags = I2C_M_RD;
    messages[1].len = 1;
    messages[1].buf = receiveBuffer;

    i2c_rdwr_ioctl_data data = {};

    data.msgs = messages;
    data.nmsgs = 2;

    if (ioctl(file, I2C_RDWR, &data) < 0)
    {

    }

    return receiveBuffer[0];
}

void I2CDevice::updateRegister(std::uint8_t registerAddress, std::uint8_t mask, std::uint8_t value) const
{
    std::uint8_t currentValue = readRegister(registerAddress);

    std::uint8_t nextValue = (currentValue & ~mask) | (value & mask);

    if (nextValue != currentValue)
    {
        writeRegister(registerAddress, nextValue);
    }
}
