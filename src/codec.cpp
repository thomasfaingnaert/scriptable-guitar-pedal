#include "codec.h"

#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>


Codec::Codec(unsigned int adapter, bool chipAddressSuffix)
{
    std::string filename = "/dev/i2c-" + std::to_string(adapter);

    file = open(filename.c_str(), O_RDWR);

    if (file < 0)
    {

    }

    int chipAddress = CHIP_ADDRESS_PREFIX << 1 | chipAddressSuffix;

    if (ioctl(file, I2C_SLAVE, chipAddress) < 0)
    {

    }
}

Codec::~Codec()
{
    close(file);
}

void Codec::writeRegister(Codec::Register reg, uint8_t value) const
{

}

uint8_t Codec::readRegister(Codec::Register reg) const
{
    return 0;
}

void Codec::updateRegister(Codec::Register reg, uint8_t mask, uint8_t value) const
{

}

