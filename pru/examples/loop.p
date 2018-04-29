; Run a simple delay loop on the PRU

.setcallreg r29.w0
.origin 0
.entrypoint start

#define PRU0_ARM_INTERRUPT 19

#define DELAY_COUNT 50000000
#define LOOP_ITERATIONS 20

start:
    mov r1, LOOP_ITERATIONS

main:
    call delay
    sub r1, r1, 1
    qbne main, r1, 0
    mov r31.b0, PRU0_ARM_INTERRUPT + 16 ; notify ARM we are done
    halt

delay:
    mov r0, DELAY_COUNT

delay_loop:
    sub r0, r0, 1
    qbne delay_loop, r0, 0
    ret
