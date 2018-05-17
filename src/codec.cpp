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

    // Reset the codec
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::ofstream gpio50_value("/sys/class/gpio/gpio50/value");
    gpio50_value << 1;
    gpio50_value.close();

    // MCLK will appear prior to 1 ms from the release of not reset
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // Activate the control port and place the part in power-down
    writeRegister(MODE_CONTROL_2, 0x03);

    // Select master mode
    writeRegister(MODE_CONTROL_1, 0x08);

    // Power up the device
    writeRegister(MODE_CONTROL_2, 0x02);

    // The power-up sequence requires approximately 85 us
    std::this_thread::sleep_for(std::chrono::microseconds(85));
}
