.origin 0
.entrypoint start

#include "mcasp.p"

#define CONSTANT_PRU_ICSS_CONFIG C4

start:
    /* Enable the Open Core Protocol master port, which is used to interconnect all peripherals on the SoC */
    lbco    r0, CONSTANT_PRU_ICSS_CONFIG, 4, 4
    clr     r0, r0, 4
    sbco    r0, CONSTANT_PRU_ICSS_CONFIG, 4, 4

    /* Load the McASP0 base address */
    mov     MCASP_BASE_ADDRESS_REGISTER, MCASP0_BASE_ADDRESS

    