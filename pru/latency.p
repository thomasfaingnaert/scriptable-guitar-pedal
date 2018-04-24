.origin 0
.entrypoint start

#define CONST_PRUCFG C4
#define PRUSS_PRU_ARM_INTERRUPT 35
#define RTDM_PRU_ARM_INTERRUPT 36

start:
    /* Enable the Open Core Protocol master port, which is used to interconnect all peripherals on the SoC */
    lbco    r0, CONST_PRUCFG, 4, 4
    clr     r0, r0, 4
    sbco    r0, CONST_PRUCFG, 4, 4

loop_begin:
    /* Wait until shared_memory[0] == 1 */
wait_for_data:
    mov     r3, 0x10000
    lbbo    r4, r3, 0, 4
    qbne    wait_for_data, r4, 1

    /* Set to zero */
    mov     r4, 0
    sbbo    r4, r3, 0, 4

    /* Send interrupt */
    mov r31.b0, RTDM_PRU_ARM_INTERRUPT
    mov r31.b0, PRUSS_PRU_ARM_INTERRUPT

    jmp loop_begin

    /* End of program */
    halt
