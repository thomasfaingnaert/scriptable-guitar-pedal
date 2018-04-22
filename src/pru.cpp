#include <fcntl.h>
#include <pruss_intc_mapping.h>
#include <prussdrv.h>
#include <stdexcept>
#include <sys/mman.h>
#include <unistd.h>

#include "pru.h"

PRU::PRU()
{
    // Initialise and allocate memory for the PRU Subsystem driver
    prussdrv_init();

    // Open event out and initialise memory mapping
    if (prussdrv_open(PRU_EVTOUT_0) == -1)
    {
        throw std::runtime_error("prussdrv_open() failed");
    }

    // Initialise and enable the PRU Interrupt Controller (PIC)
    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
    prussdrv_pruintc_init(&pruss_intc_initdata);
}

PRU::~PRU()
{
    prussdrv_pru_disable(PRU0);
    prussdrv_exit();
}

void PRU::executeProgram(const std::string& filename) const
{
    // Disable PRU, write instructions to PRU instruction RAM, enable PRU and start executing instructions
    if (prussdrv_exec_program(PRU0, filename.c_str()) < 0)
    {
        throw std::runtime_error("Error executing " + filename);
    }
}

void PRU::waitForInterrupt() const
{
    // Wait for PRU event out
    prussdrv_pru_wait_event(PRU_EVTOUT_0);

    // Clear the system event
    prussdrv_pru_clear_event(PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
}

ulong* PRU::setupSharedMemory() const
{
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    ulong* sharedMemory = static_cast<ulong*>(mmap(0, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ADDRESS + SHARED_MEMORY_OFFSET));
    return sharedMemory;
}
