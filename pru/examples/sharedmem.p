.origin 0
.entrypoint start

#define CONST_PRUCFG C4
#define PRU0_ARM_INTERRUPT 19

start:
    /* Enable the Open Core Protocol master port, which is used to interconnect all peripherals on the SoC */
    lbco    r0, CONST_PRUCFG, 4, 4
    clr     r0, r0, 4
    sbco    r0, CONST_PRUCFG, 4, 4

loop_begin:
    /* Wait until shared_memory[0] == 1, i.e. data available */
wait_for_data:
    mov     r3, 0x10000
    lbbo    r4, r3, 0, 4
    qbne    wait_for_data, r4, 1

    /* Add one to sharedmem[1] */
    lbbo    r4, r3, 4, 4
    add     r4, r4, 1
    sbbo    r4, r3, 4, 4

    /* Set shared_memory[0] = 0, i.e. data processed */
    mov     r4, 0
    sbbo    r4, r3, 0, 4

    /* Send interrupt to ARM */
    mov     r31.b0, PRU0_ARM_INTERRUPT+16

    jmp loop_begin

    /* End of program */
    halt
