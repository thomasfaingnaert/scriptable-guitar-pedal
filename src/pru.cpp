#include <cstring>
#include <fcntl.h>
#include <pruss_intc_mapping.h>
#include <prussdrv.h>
#include <stdexcept>
#include <sys/mman.h>
#include <unistd.h>

#include "pru.h"

Pru::Pru()
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

    // Open the RTDM Driver
    rtdm_file_descriptor = open(RTDM_DRIVER.c_str(), O_RDWR);
    if (rtdm_file_descriptor < 0)
    {
        std::string errorMsg = "Failed to open RTDM Driver: " + std::to_string(errno) + " " + std::string(std::strerror(errno)) + "\nDid you sudo modprobe rtdm_pruss_irq?";
        throw std::runtime_error(errorMsg);
    }
}

Pru::~Pru()
{
    prussdrv_pru_disable(PRU0);
    prussdrv_exit();

    // Close RTDM Driver
    close(rtdm_file_descriptor);
}

void Pru::executeProgram(const std::string& filename) const
{
    // Disable PRU, write instructions to PRU instruction RAM, enable PRU and start executing instructions
    if (prussdrv_exec_program(PRU0, filename.c_str()) < 0)
    {
        throw std::runtime_error("Error executing " + filename);
    }
}

void Pru::waitForInterrupt() const
{
    // Read from RTDM driver to wait for interrupt
    int val;
    read(rtdm_file_descriptor, &val, sizeof(val));
}

volatile ulong* Pru::setupSharedMemory() const
{
    ulong* sharedMemory;

    prussdrv_map_prumem(PRUSS0_SHARED_DATARAM, (void**)&sharedMemory);

    return sharedMemory;
}
