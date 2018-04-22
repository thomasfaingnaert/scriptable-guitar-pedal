#include <cstring>
#include <cstdlib>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <iostream>
#include <pthread.h>
#include <thread>
#include <chrono>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define PRU_ADDR 0x4a300000
#define SHAREDRAM_OFFSET 0x10000

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " filename.bin" << std::endl;
        return EXIT_SUCCESS;
    }

    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    ulong* sharedmem = (ulong*) mmap(0, 0x10000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ADDR + SHAREDRAM_OFFSET);
    // Initialise ACK to 0
    sharedmem[0] = 0;

    prussdrv_init();

    if (prussdrv_open(PRU_EVTOUT_0) == -1)
    {
        std::cerr << "prussdrv_open() failed" << std::endl;
        return EXIT_FAILURE;
    }

    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

    prussdrv_pruintc_init(&pruss_intc_initdata);

    std::cout << "Running program and waiting for termination..." << std::endl;

    if (prussdrv_exec_program(PRU0, argv[1]) < 0)
    {
        std::cerr << "Error loading " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    while (true)
    {
        // First read input
        int input;
        std::cout << "enter number: ";
        std::cin >> input;

        // sharedmem[0] = ready? and sharedmem[1] = data
        sharedmem[1] = input;
        sharedmem[0] = 1;

        std::cout << "Waiting for the PRU to write response..." << std::endl;

        // wait for interrupt
        prussdrv_pru_wait_event(PRU_EVTOUT_0);
        prussdrv_pru_clear_event(PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);

        std::cout << "PRU gave result: " << sharedmem[1] << std::endl;
    }

    std::cout << "Done" << std::endl;
    prussdrv_pru_disable(PRU0);
    prussdrv_exit();

    return EXIT_SUCCESS;
}

