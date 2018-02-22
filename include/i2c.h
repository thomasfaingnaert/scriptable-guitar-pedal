#ifndef SCRIPTABLE_GUITAR_PEDAL_I2C_H
#define SCRIPTABLE_GUITAR_PEDAL_I2C_H

#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

class I2C {
public:
    I2C();
    I2C(int adapter);

    void set_device(int address);

    virtual ~I2C();
private:
    std::string adapter_to_filename();

    int adapter;
    int file;
};


#endif
