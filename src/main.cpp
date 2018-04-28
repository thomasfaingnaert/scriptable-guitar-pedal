#include <iostream>
#include <pthread.h>
#include <chrono>
#include <unistd.h>
#include <cobalt/stdio.h>

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

    sharedMemory[0] = 0;
    pru.executeProgram(argv[1]);

    auto begin = std::chrono::high_resolution_clock::now();
    while (source->generate_next()) ;
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "took " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " ms\n";

    auto begin = std::chrono::high_resolution_clock::now();
    for (unsigned int i = 0; i < 1000000; ++i)
    {
        // Set memory to 1
        sharedMemory[0] = 1;
        pru.waitForInterrupt();
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Took in total " << std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count() << " us\n";

    //rt_printf("Finished");

    return EXIT_SUCCESS;
}

