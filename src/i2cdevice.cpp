#include "i2cdevice.h"

#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <string>

I2CDevice::I2CDevice(unsigned int adapter, uint16_t deviceAddress) : deviceAddress(deviceAddress)
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

void I2CDevice::writeRegister(uint8_t registerAddress, uint8_t value) const
{
    uint8_t sendBuffer[] = {registerAddress, value};

    i2c_msg messages[] = {
            {
                    .addr = deviceAddress,
                    .flags = 0,
                    .len = 2,
                    .buf = sendBuffer
            }
    };

    i2c_rdwr_ioctl_data data = {
            .msgs = messages,
            .nmsgs = 1
    };

    if (ioctl(file, I2C_RDWR, &data) < 0)
    {

    }
}

uint8_t I2CDevice::readRegister(uint8_t registerAddress) const
{
    uint8_t sendBuffer[] = {registerAddress};
    uint8_t receiveBuffer[] = {0};

    i2c_msg messages[] = {
            {
                    .addr = deviceAddress,
                    .flags = 0,
                    .len = 1,
                    .buf = sendBuffer

            },
            {
                    .addr = deviceAddress,
                    .flags = I2C_M_RD,
                    .len = 1,
                    .buf = receiveBuffer
            }
    };

    i2c_rdwr_ioctl_data data = {
            .msgs = messages,
            .nmsgs = 2
    };

    if (ioctl(file, I2C_RDWR, &data) < 0)
    {

    }

    return receiveBuffer[0];
}
