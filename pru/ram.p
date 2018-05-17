.origin 0
.entrypoint start

#include "ram.hp"

#define CONSTANT_PRU_ICSS_CONFIG C4

start:
    // Enable the Open Core Protocol master port, which is used to interconnect all peripherals on the SoC
    lbco    r0, CONSTANT_PRU_ICSS_CONFIG, 4, 4
    clr     r0, r0, 4
    sbco    r0, CONSTANT_PRU_ICSS_CONFIG, 4, 4

    mov     r2, 0
    mov     r10, 256
loop_beg:
    add     r2, r2, 1
    ram_input_push
    qbne    loop_beg, r2, r10

infinite_loop_beg:
    ram_output_pop
    ram_input_push
    jmp infinite_loop_beg

    halt
