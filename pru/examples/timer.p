.origin 0
.entrypoint start

#define PRU0_ARM_INTERRUPT 19

start:
    mov r0, 20000000
start_loop: ; 2 * 20 000 000 instructions * 5 ns / instruction = 0.2 seconds timer interval
    sub r0, r0, 1
    qbne start_loop, r0, 0

    mov r31.b0, PRU0_ARM_INTERRUPT + 16 ; notify ARM we are done
    jmp start

    halt
