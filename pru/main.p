.origin 0
.entrypoint start

#include "mcasp.hp"

#define CONSTANT_PRU_ICSS_CONFIG C4

start:
    // Enable the Open Core Protocol master port, which is used to interconnect all peripherals on the SoC
    lbco    r0, CONSTANT_PRU_ICSS_CONFIG, 4, 4
    clr     r0, r0, 4
    sbco    r0, CONSTANT_PRU_ICSS_CONFIG, 4, 4

    // Load the McASP0 base address
    mov     MCASP_BASE_ADDRESS_REGISTER, MCASP0_BASE_ADDRESS

    // Reset the McASP to default values
    mcasp_register_write    MCASP_GBLCTL_OFFSET, 0x00000000

    // Smart-idle mode
    #define MCASP_IDLEMODE 0x2

    #define MCASP_PWRIDLESYSCONFIG (MCASP_IDLEMODE << 0)

    mcasp_register_write    MCASP_PWRIDLESYSCONFIG_OFFSET, MCASP_PWRIDLESYSCONFIG

    // The 8 most significant bits of receive data are masked out and then padded with the selected bit pad value (RPAD and RPBIT bits in RFMT)
    #define MCASP_RMASK 0x00FFFFFF

    mcasp_register_write    MCASP_RMASK_OFFSET, MCASP_RMASK

    // 0-bit delay
    #define MCASP_RDATDLY 0x0
    // Bitstream is MSB first
    #define MCASP_RRVRS 0x1
    // Pad extra bits with one of the bits from the word as specified by RPBIT bits
    #define MCASP_RPAD 0x2
    // Pad with bit 23
    #define MCASP_RPBIT 23
    // Slot size is 32 bits
    #define MCASP_RSSZ 0xF
    // Reads from XRBUF[n] originate from the data port
    #define MCASP_RBUSEL 0x0
    // Rotate right by 8 bit positions
    #define MCASP_RROT 0x2

    #define MCASP_RFMT (MCASP_RDATDLY << 16) | (MCASP_RRVRS << 15) | (MCASP_RPAD << 13) | (MCASP_RPBIT << 8) | (MCASP_RSSZ << 4) | (MCASP_RBUSEL << 3) | (MCASP_RROT << 0)

    mcasp_register_write    MCASP_RFMT_OFFSET, MCASP_RFMT

    // 2-slot TDM
    #define MCASP_RMOD 0x2
    // Single word width of the receive frame sync during its active period
    #define MCASP_FRWID 0x1
    // Externally-generated receive frame sync
    #define MCASP_FSRM 0x0
    // A rising edge on the receive frame sync indicates the beginning of a frame
    #define MCASP_FSRP 0x0

    #define MCASP_AFSRCTL (MCASP_RMOD << 7) | (MCASP_FRWID << 4) | (MCASP_FSRM << 1) | (MCASP_FSRP << 0)

    mcasp_register_write    MCASP_AFSRCTL_OFFSET, MCASP_AFSRCTL

    // This is irrelevant, because MCASP_ASYNC = 0x0
    mcasp_register_write    MCASP_ACLKRCTL_OFFSET, 0x00000000
    // This has no effect
    mcasp_register_write    MCASP_AHCLKRCTL_OFFSET, 0x00000000

    // Receive TDM time slot 0 and 1 are active
    #define MCASP_RTDMS 0x3

    #define MCASP_RTDM (MCASP_RTDMS << 0)

    mcasp_register_write    MCASP_RTDM_OFFSET, MCASP_RTDM

    // Disable all receive interrupts
    mcasp_register_write    MCASP_RINTCTL_OFFSET, 0x00000000

    // Do not use the receive clock failure detection circuit
    mcasp_register_write    MCASP_RCLKCHK_OFFSET, 0x00000000

    // The 8 most significant bits of transmit data are masked out and then padded with the selected bit pad value (XPAD and XPBIT bits in XFMT)
    #define MCASP_XMASK 0xFFFFFF00

    mcasp_register_write    MCASP_XMASK_OFFSET, MCASP_XMASK

    // 0-bit delay
    #define MCASP_XDATDLY 0x0
    // Bitstream is MSB first
    #define MCASP_XRVRS 0x1
    // Pad extra bits with 0
    #define MCASP_XPAD 0x0
    // This has no effect, because MCASP_XPAD != 0x2
    #define MCASP_XPBIT 0
    // Slot size is 32 bits
    #define MCASP_XSSZ 0xF
    // Writes to XRBUF[n] originate from the data port
    #define MCASP_XBUSEL 0x0
    // Rotate right by 24 bit positions
    #define MCASP_XROT 0x6

    #define MCASP_XFMT (MCASP_XDATDLY << 16) | (MCASP_XRVRS << 15) | (MCASP_XPAD << 13) | (MCASP_XPBIT << 8) | (MCASP_XSSZ << 4) | (MCASP_XBUSEL << 3) | (MCASP_XROT << 0)

    mcasp_register_write    MCASP_XFMT_OFFSET, MCASP_XFMT

    // 2-slot TDM
    #define MCASP_XMOD 0x2
    // Single word width of the transmit frame sync during its active period
    #define MCASP_FXWID 0x1
    // Externally-generated transmit frame sync
    #define MCASP_FSXM 0x0
    // A rising edge on the transmit frame sync indicates the beginning of a frame
    #define MCASP_FSXP 0x0

    #define MCASP_AFSXCTL (MCASP_XMOD << 7) | (MCASP_FXWID << 4) | (MCASP_FSXM << 1) | (MCASP_FSXP << 0)

    mcasp_register_write    MCASP_AFSXCTL_OFFSET, MCASP_AFSXCTL

    // External receiver samples data on the rising edge of the serial clock
    #define MCASP_CLKXP 0x1
    // Transmit clock and frame sync provides the source for both the transmit and receive sections
    #define MCASP_ASYNC 0x0
    // External transmit clock source from ACLKX pin
    #define MCASP_CLKXM 0x0
    // This has no effect, because MCASP_CLKXM = 0x0
    #define MCASP_CLKXDIV 0x0

    #define MCASP_ACLKXCTL (MCASP_CLKXP << 7) | (MCASP_ASYNC << 6) | (MCASP_CLKXM << 5) | (MCASP_CLKXDIV << 0)

    mcasp_register_write    MCASP_ACLKXCTL_OFFSET, MCASP_ACLKXCTL

    // External transmit high-frequency clock source from AHCLKX
    #define MCASP_HCLKXM 0x0
    // This has no effect
    #define MCASP_HCLKXP 0x0
    // This has no effect
    #define MCASP_HCLKXDIV 0x0

    #define MCASP_AHCLKXCTL (MCASP_HCLKXM << 15) | (MCASP_HCLKXP << 14) | (MCASP_HCLKXDIV << 0)

    mcasp_register_write    MCASP_AHCLKXCTL_OFFSET, MCASP_AHCLKXCTL

    // Transmit TDM time slot 0 and 1 are active
    #define MCASP_XTDMS 0x3

    #define MCASP_XTDM (MCASP_XTDMS << 0)

    mcasp_register_write    MCASP_XTDM_OFFSET, MCASP_XTDM

    // Disable all transmit interrupts
    mcasp_register_write    MCASP_XINTCTL_OFFSET, 0x00000000

    // Do not use the transmit clock failure detection circuit
    mcasp_register_write    MCASP_XCLKCHK_OFFSET, 0x00000000

    // Drive on pin is logic high
    #define MCASP_DISMOD_0 0x3
    // Serializer is receiver
    #define MCASP_SRMOD_0 0x2

    #define MCASP_SRCTL_0 (MCASP_DISMOD_0 << 2) | (MCASP_SRMOD_0 << 0)

    mcasp_register_write_extended   MCASP_SRCTL_0_OFFSET, MCASP_SRCTL_0

    // Drive on pin is logic high
    #define MCASP_DISMOD_2 0x3
    // Serializer is transmitter
    #define MCASP_SRMOD_2 0x1

    #define MCASP_SRCTL_2 (MCASP_DISMOD_2 << 2) | (MCASP_SRMOD_2 << 0)

    mcasp_register_write_extended   MCASP_SRCTL_2_OFFSET, MCASP_SRCTL_2

    // AFSR functions as a GPIO pin
    #define MCASP_PFUNC_AFSR 0x1
    // AHCLKR functions as a GPIO pin
    #define MCASP_PFUNC_AHCLKR 0x1
    // ACLKR functions as a GPIO pin
    #define MCASP_PFUNC_ACLKR 0x1
    // AFSX functions as a McASP pin
    #define MCASP_PFUNC_AFSX 0x0
    // AHCLKX functions as a McASP pin
    #define MCASP_PFUNC_AHCLKX 0x0
    // ACLKX functions as a McASP pin
    #define MCASP_PFUNC_ACLKX 0x0
    // AMUTE functions as a McASP pin
    #define MCASP_PFUNC_AMUTE 0x0
    // AXR0 functions as a McASP pin
    // AXR1 functions as a GPIO pin
    // AXR2 functions as a McASP pin
    // AXR3 functions as a GPIO pin
    #define MCASP_PFUNC_AXR 0x5

    #define MCASP_PFUNC (MCASP_PFUNC_AFSR << 31) | (MCASP_PFUNC_AHCLKR << 30) | (MCASP_PFUNC_ACLKR << 29) | (MCASP_PFUNC_AFSX << 28) | (MCASP_PFUNC_AHCLKX << 27) | (MCASP_PFUNC_ACLKX << 26) | (MCASP_PFUNC_AMUTE << 25) | (MCASP_PFUNC_AXR << 0)

    mcasp_register_write    MCASP_PFUNC_OFFSET, MCASP_PFUNC

    // AFSR functions as an input
    #define MCASP_PDIR_AFSR 0x0
    // AHCLKR functions as an input
    #define MCASP_PDIR_AHCLKR 0x0
    // ACLKR functions as an input
    #define MCASP_PDIR_ACLKR 0x0
    // AFSX functions as an input
    #define MCASP_PDIR_AFSX 0x0
    // AHCLKX functions as an input
    #define MCASP_PDIR_AHCLKX 0x0
    // ACLKX functions as an input
    #define MCASP_PDIR_ACLKX 0x0
    // AMUTE functions as an output
    #define MCASP_PDIR_AMUTE 0x1
    // AXR0 functions as an input
    // AXR1 functions as an input
    // AXR2 functions as an output
    // AXR3 functions as an input
    #define MCASP_PDIR_AXR 0x4

    #define MCASP_PDIR (MCASP_PDIR_AFSR << 31) | (MCASP_PDIR_AHCLKR << 30) | (MCASP_PDIR_ACLKR << 29) | (MCASP_PDIR_AFSX << 28) | (MCASP_PDIR_AHCLKX << 27) | (MCASP_PDIR_ACLKX << 26) | (MCASP_PDIR_AMUTE << 25) | (MCASP_PDIR_AXR << 0)

    mcasp_register_write    MCASP_PDIR_OFFSET, MCASP_PDIR

    // DIT mode is disabled
    #define MCASP_DITEN 0x0

    #define MCASP_DITCTL (MCASP_DITEN << 0)

    mcasp_register_write    MCASP_DITCTL_OFFSET, MCASP_DITCTL

    // Loopback mode is disabled
    #define MCASP_DLBEN 0x0

    #define MCASP_DLBCTL (MCASP_DLBEN << 0)

    mcasp_register_write    MCASP_DLBCTL_OFFSET, MCASP_DLBCTL

    // AMUTE pin is disabled
    #define MCASP_MUTEN 0x0

    #define MCASP_AMUTE (MCASP_MUTEN << 0)

    mcasp_register_write    MCASP_AMUTE_OFFSET, MCASP_AMUTE

    // Receive high-frequency clock divider is running
    mcasp_register_set_bit_and_poll MCASP_GBLCTL_OFFSET, 1
    // Transmit high-frequency clock divider is running
    mcasp_register_set_bit_and_poll MCASP_GBLCTL_OFFSET, 9

    // Clear the receiver status register
    mcasp_register_write    MCASP_RSTAT_OFFSET, 0xFFFF
    // Clear the transmitter status register
    mcasp_register_write    MCASP_XSTAT_OFFSET, 0xFFFF

    // Receive serializers are active
    mcasp_register_set_bit_and_poll MCASP_GBLCTL_OFFSET, 2
    // Transmit serializers are active
    mcasp_register_set_bit_and_poll MCASP_GBLCTL_OFFSET, 10

    // Write to XBUF
    mcasp_register_write_extended MCASP_XBUF_2_OFFSET, 0x00000000

    // Receive state machine is released from reset
    mcasp_register_set_bit_and_poll MCASP_GBLCTL_OFFSET, 3
    // Transmit state machine is released from reset
    mcasp_register_set_bit_and_poll MCASP_GBLCTL_OFFSET, 11

    // Receive frame sync generator is active
    mcasp_register_set_bit_and_poll MCASP_GBLCTL_OFFSET, 4
    // Transmit frame sync generator is active
    mcasp_register_set_bit_and_poll MCASP_GBLCTL_OFFSET, 12

    halt
