#include <cstring>
#include <cstdlib>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <iostream>
#include <pthread.h>
#include <thread>
#include <chrono>

void *pruevtout0_thread(void *arg)
{
    static auto start = std::chrono::high_resolution_clock::now();

    sched_param params;
    params.sched_priority = 99;
    if (pthread_setschedparam(pthread_self(), SCHED_RR, &params))
    {
        std::cerr << "Failed to set priority for thread: " << std::strerror(errno) << "\n";
    }

    while (1)
    {
        prussdrv_pru_wait_event(PRU_EVTOUT_0);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "took " << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count() << " us\n";
        start = end;
        prussdrv_pru_clear_event(PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
    }
}

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

    pthread_t irq_thread;
    if (pthread_create(&irq_thread, nullptr, pruevtout0_thread, nullptr))
    {
        std::cerr << "Error creating IRQ thread" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Running program and waiting for termination..." << std::endl;

    if (prussdrv_exec_program(PRU0, argv[1]) < 0)
    {
        std::cerr << "Error loading " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    prussdrv_pru_wait_event(PRU_EVTOUT_0);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "Done" << std::endl;

    prussdrv_pru_disable(PRU0);
    prussdrv_exit();

    return EXIT_SUCCESS;
}

