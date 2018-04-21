#include <cstdlib>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " filename.bin" << std::endl;
        return EXIT_SUCCESS;
    }

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

    prussdrv_pru_wait_event(PRU_EVTOUT_0);

    std::cout << "Done" << std::endl;

    prussdrv_pru_disable(PRU0);
    prussdrv_exit();

    return EXIT_SUCCESS;
}

