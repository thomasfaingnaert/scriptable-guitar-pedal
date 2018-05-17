#include <cmath>
#include <iostream>
#include <thread>
#include <chrono>

#include "prudevice.h"

PruDevice::PruDevice()
{
    sharedMemory = pru.setupSharedMemory();

    // Set iterators
    sharedMemory[INPUT_BEGIN_OFFSET] = sharedMemory[INPUT_END_OFFSET] = 0;
    sharedMemory[OUTPUT_BEGIN_OFFSET] = sharedMemory[OUTPUT_END_OFFSET] = 0;

    pru.executeProgram("main.bin");
}

void PruDevice::push(const std::array<float, Constants::BLOCK_SIZE>& data)
{
    for (float f : data)
    {
        // Convert to integer
        ulong i = static_cast<ulong>(f * std::pow(2, 24));

        // Read current begin and end pointer
        ulong begin = sharedMemory[OUTPUT_BEGIN_OFFSET];
        ulong end = sharedMemory[OUTPUT_END_OFFSET];

        // Wait until buffer is no longer full
        while ((end + 1) % BUFFER_SIZE == begin)
        {
            begin = sharedMemory[OUTPUT_BEGIN_OFFSET];
            end = sharedMemory[OUTPUT_END_OFFSET];
        }

        // Write result
        sharedMemory[OUTPUT_DATA_BEGIN_OFFSET + end] = i;

        // Increment end pointer
        end = (end + 1) % BUFFER_SIZE;
        sharedMemory[OUTPUT_END_OFFSET] = end;
    }
}

void PruDevice::generate_next()
{
    std::array<float, Constants::BLOCK_SIZE> data;

    for (unsigned int i = 0; i < Constants::BLOCK_SIZE; ++i)
    {
        // Read current begin and end pointer
        ulong begin = sharedMemory[INPUT_BEGIN_OFFSET];
        ulong end = sharedMemory[INPUT_END_OFFSET];

        // Wait until there is a new sample available
        while (begin == end)
        {
            pru.waitForInterrupt();
            begin = sharedMemory[INPUT_BEGIN_OFFSET];
            end = sharedMemory[INPUT_END_OFFSET];
        }

        // Read sample
        data[i] = static_cast<float>(sharedMemory[INPUT_DATA_BEGIN_OFFSET + begin]) / std::pow(2, 24);

        // Increment begin pointer
        begin = (begin + 1) % BUFFER_SIZE;
        sharedMemory[INPUT_BEGIN_OFFSET] = begin;
    }

    generate(data);
}

