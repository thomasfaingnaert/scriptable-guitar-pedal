#include <iostream>
#include <pthread.h>
#include <chrono>

#include "pru.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " filename.bin" << std::endl;
        return EXIT_SUCCESS;
    }

    sched_param param;
    param.sched_priority = 99;
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);

    PRU pru;
    ulong* sharedMemory = pru.setupSharedMemory();

    sharedMemory[0] = 1;
    pru.executeProgram(argv[1]);

    // Set memory to 0
    sharedMemory[0] = 0;

    auto start = std::chrono::high_resolution_clock::now();

    // Wait for interrupt
    pru.waitForInterrupt();
    auto end = std::chrono::high_resolution_clock::now();
    int val = sharedMemory[0];

    std::cout << "Counter is at: " << val << std::endl;
    std::cout << "Took " << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << " us\n";

    return EXIT_SUCCESS;
}

