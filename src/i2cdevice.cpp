#include "i2cdevice.h"

#include <string>

#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>


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
    struct i2c_msg messages[1];

    messages[0].addr = deviceAddress;
    messages[0].flags = 0;

    uint8_t sendBuffer[2];

    sendBuffer[0] = registerAddress;
    sendBuffer[1] = value;

    messages[0].len = sizeof(sendBuffer);
    messages[0].buf = sendBuffer;

    struct i2c_rdwr_ioctl_data data;

    data.msgs = messages;
    data.nmsgs = 1;

    if (ioctl(file, I2C_RDWR, &data) < 0)
    {

    }
}
