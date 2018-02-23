#ifndef SCRIPTABLE_GUITAR_PEDAL_I2C_H
#define SCRIPTABLE_GUITAR_PEDAL_I2C_H

extern "C"
{
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}

class I2C {
public:
    I2C();
    I2C(int adapter);

    void set_device(int address);

    void test_write();

    virtual ~I2C();
private:
    std::string adapter_to_filename();

    int adapter;
    int file;
};


#endif
