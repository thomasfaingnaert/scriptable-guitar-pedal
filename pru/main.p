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
    // Reads from XRBUF[n] originate on data port
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

    halt
