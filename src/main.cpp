#include <iostream>

#include "pru.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " filename.bin" << std::endl;
        return EXIT_SUCCESS;
    }

    PRU pru;
    ulong* sharedMemory = pru.setupSharedMemory();

    pru.executeProgram(argv[1]);

    while (true)
    {
        // First read input
        int input;
        std::cout << "enter number: ";
        std::cin >> input;

        // sharedMemory[0] = ready? and sharedMemory[1] = data
        sharedMemory[1] = input;
        sharedMemory[0] = 1;

        std::cout << "Waiting for the PRU to write response..." << std::endl;

        // wait for interrupt
        pru.waitForInterrupt();

        std::cout << "PRU gave result: " << sharedMemory[1] << std::endl;
    }

    return EXIT_SUCCESS;
}

