#ifndef PRU_H_E1NOUCLR
#define PRU_H_E1NOUCLR

#include <string>

class Pru
{
    public:
        Pru();
        ~Pru();
        void executeProgram(const std::string& filename) const;
        void waitForInterrupt() const;
        volatile ulong* setupSharedMemory() const;

    private:
        const std::string RTDM_DRIVER = "/dev/rtdm/rtdm_pruss_irq_0";
        int rtdm_file_descriptor;
};

#endif /* end of include guard: PRU_H_E1NOUCLR */
