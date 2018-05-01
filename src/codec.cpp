#include <iostream>
#include <fstream>
#include <thread>

#include "codec.h"

Codec::Codec(unsigned int adapter, uint16_t deviceAddress) : I2CDevice(adapter, deviceAddress)
{

}

void Codec::setFunctionalMode(Codec::FunctionalMode functionalMode)
{
}

Codec::FunctionalMode Codec::getFunctionalMode() const
{

}
