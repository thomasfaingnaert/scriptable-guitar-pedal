#ifndef PRUDEVICE_H_GZ1JA6SL
#define PRUDEVICE_H_GZ1JA6SL

#include "pru.h"
#include "sink.h"
#include "source.h"

class PruDevice : public Source<float>, public Sink<float>
{
    public:
        PruDevice();
        virtual void push(const std::array<float, Constants::BLOCK_SIZE>& data);
        void generate_next();

    private:
        Pru pru;
        volatile ulong* sharedMemory;

        static constexpr unsigned int BUFFER_SIZE = 1024;                                               // Size of the input and output buffer (in 32-bit samples)

        static constexpr unsigned int INPUT_BEGIN_OFFSET = 0;                                           // The iterator to the beginning of the input buffer
        static constexpr unsigned int INPUT_END_OFFSET = 1;                                             // The iterator to the end of the input buffer
        static constexpr unsigned int INPUT_DATA_BEGIN_OFFSET = 2;                                      // The beginning of the contents of the input buffer
        static constexpr unsigned int INPUT_DATA_END_OFFSET = INPUT_DATA_BEGIN_OFFSET + BUFFER_SIZE;    // The end of the contents of the input buffer

        static constexpr unsigned int OUTPUT_BEGIN_OFFSET = INPUT_DATA_END_OFFSET;                      // The iterator to the beginning of the output buffer
        static constexpr unsigned int OUTPUT_END_OFFSET = OUTPUT_BEGIN_OFFSET + 1;                      // The iterator to the end of the output buffer
        static constexpr unsigned int OUTPUT_DATA_BEGIN_OFFSET = OUTPUT_END_OFFSET + 1;                 // The beginning of the contents of the output buffer
        static constexpr unsigned int OUTPUT_DATA_END_OFFSET = OUTPUT_DATA_BEGIN_OFFSET + BUFFER_SIZE;  // The end of the contents of the output buffer
};

#endif /* end of include guard: PRUDEVICE_H_GZ1JA6SL */
