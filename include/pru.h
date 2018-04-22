#ifndef PRU_CPP_KPJ9JOMT
#define PRU_CPP_KPJ9JOMT

#include <string>

class PRU
{
    public:
        PRU();
        ~PRU();
        void executeProgram(const std::string& filename) const;
        void waitForInterrupt() const;
        ulong* setupSharedMemory() const;

    private:
        static constexpr unsigned int PRU_ADDRESS = 0x4A300000;
        static constexpr unsigned int SHARED_MEMORY_OFFSET = 0x10000;
        static constexpr unsigned int SHARED_MEMORY_SIZE = 0x10000;
};

#endif /* end of include guard: PRU_CPP_KPJ9JOMT */
