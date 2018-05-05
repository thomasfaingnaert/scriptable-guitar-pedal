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
        ulong* setupSharedMemory() const;

    private:
        static constexpr unsigned int PRU_ADDRESS = 0x4A300000;
        static constexpr unsigned int SHARED_MEMORY_OFFSET = 0x10000;
        static constexpr unsigned int SHARED_MEMORY_SIZE = 0x10000;

        const std::string RTDM_DRIVER = "/dev/rtdm/rtdm_pruss_irq_0";
        int rtdm_file_descriptor;
};

#endif /* end of include guard: PRU_H_E1NOUCLR */
