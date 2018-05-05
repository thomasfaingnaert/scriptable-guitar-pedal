#include <cmath>
#include <iostream>
#include <thread>
#include <chrono>

#include "prudevice.h"

PruDevice::PruDevice()
{
    sharedMemory = pru.setupSharedMemory();

    // Set iterators
    sharedMemory[OFFSET_INPUT_BEGIN] = sharedMemory[OFFSET_INPUT_END] = 0;
    sharedMemory[OFFSET_OUTPUT_BEGIN] = sharedMemory[OFFSET_OUTPUT_END] = 0;

    pru.executeProgram("ram.bin");
}

void PruDevice::push(const std::array<float, Constants::BLOCK_SIZE>& data)
{
    for (float f : data)
    {
        // Convert to integer
        ulong i = static_cast<ulong>(f * std::pow(2, 24));

        // Read current begin and end pointer
        ulong begin = sharedMemory[OFFSET_OUTPUT_BEGIN];
        ulong end = sharedMemory[OFFSET_OUTPUT_END];

        // Wait until buffer is no longer full
        while ((end + 1) % BUFFER_SIZE == begin)
        {
            begin = sharedMemory[OFFSET_OUTPUT_BEGIN];
            end = sharedMemory[OFFSET_OUTPUT_END];
        }

        // Write result
        sharedMemory[OFFSET_OUTPUT_DATA_BEGIN + end] = i;

        // Increment end pointer
        end = (end + 1) % BUFFER_SIZE;
        sharedMemory[OFFSET_OUTPUT_END] = end;
    }
}

void PruDevice::generate_next()
{
    std::array<float, Constants::BLOCK_SIZE> data;

    for (unsigned int i = 0; i < Constants::BLOCK_SIZE; ++i)
    {
        // Read current begin and end pointer
        ulong begin = sharedMemory[OFFSET_INPUT_BEGIN];
        ulong end = sharedMemory[OFFSET_INPUT_END];

        // Wait until there is a new sample available
        while (begin == end)
        {
            pru.waitForInterrupt();
            begin = sharedMemory[OFFSET_INPUT_BEGIN];
            end = sharedMemory[OFFSET_INPUT_END];
        }

        // Read sample
        //data[i] = static_cast<float>(sharedMemory[OFFSET_INPUT_DATA_BEGIN + begin] / std::pow(2, 24));
        std::cout << "read sample: " << sharedMemory[OFFSET_INPUT_DATA_BEGIN + begin] << std::endl;

        // Increment begin pointer
        begin = (begin + 1) % BUFFER_SIZE;
        sharedMemory[OFFSET_INPUT_BEGIN] = begin;
    }

    generate(data);
}

