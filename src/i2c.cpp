#include <fcntl.h>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

#include "i2c.h"

I2C::I2C() : I2C(0) {}

I2C::I2C(int adapter) : adapter(adapter)
{
    std::string filename = adapter_to_filename();

    file = open(filename.c_str(), O_RDWR);

    if(file < 0)
    {
        std::cout << "error: i2c: can't open \"" << filename << "\"" << std::endl;
    }
    else
    {
        std::cout << "log: i2c: succesfully opened \"" << filename << "\"" << std::endl;
    }
}

I2C::~I2C()
{
    std::string filename = adapter_to_filename();

    if(close(file) < 0)
    {
        std::cout << "error: i2c: can't close \"" << filename << "\"" << std::endl;
    }
    else
    {
        std::cout << "log: i2c: successfully closed \"" << filename << "\"" << std::endl;
    }
}

inline std::string I2C::adapter_to_filename()
{
    return "/dev/i2c-" + std::to_string(adapter);
}

void I2C::set_device(int address)
{
    if(ioctl(file, I2C_SLAVE, address) < 0)
    {
        std::cout << "error: i2c: can't open device at address " << address << std::endl;
    }
    else
    {
        std::cout << "log: i2c: successfully opened device at address " << address << std::endl;
    }
}

void I2C::test_write()
{
    i2c_smbus_write_byte_data(file, 0x40, 0xF0);
}
