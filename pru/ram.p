.origin 0
.entrypoint start

#include "ram.hp"

#define CONSTANT_PRU_ICSS_CONFIG C4

start:
    // Enable the Open Core Protocol master port, which is used to interconnect all peripherals on the SoC
    lbco    r0, CONSTANT_PRU_ICSS_CONFIG, 4, 4
    clr     r0, r0, 4
    sbco    r0, CONSTANT_PRU_ICSS_CONFIG, 4, 4

    mov     r2, 42
    ram_input_push

    mov     r2, 17
    ram_input_push

    ram_send_interrupt

    halt
