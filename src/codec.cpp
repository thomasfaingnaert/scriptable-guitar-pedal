#include "codec.h"

#include <chrono>
#include <fstream>
#include <thread>

Codec::Codec(unsigned int adapter, uint16_t deviceAddress) : I2CDevice(adapter, deviceAddress)
{
    std::ofstream gpio_export("/sys/class/gpio/export");
    gpio_export << 50;
    gpio_export.close();

    std::ofstream gpio50_direction("/sys/class/gpio/gpio50/direction");
    gpio50_direction << "low";
    gpio50_direction.close();

    // Give the codec time to reset
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::ofstream gpio50_value("/sys/class/gpio/gpio50/value");
    gpio50_value << 1;
    gpio50_value.close();

    // Enable control port mode and power down the device
    writeRegister(MODE_CONTROL_2, 0b00000011);

    // Select master mode
    writeRegister(MODE_CONTROL_1, 0b00001000);

    // Power up the device
    writeRegister(MODE_CONTROL_2, 0b00000010);
}
