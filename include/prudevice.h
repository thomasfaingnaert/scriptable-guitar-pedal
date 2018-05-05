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

        static constexpr unsigned int BUFFER_SIZE = 4096;                                               // Size of the input and output buffer (in 32-bit samples)

        static constexpr unsigned int OFFSET_INPUT_BEGIN = 0;                                           // The iterator to the beginning of the input buffer
        static constexpr unsigned int OFFSET_INPUT_END = 1;                                             // The iterator to the end of the input buffer
        static constexpr unsigned int OFFSET_INPUT_DATA_BEGIN = 2;                                      // The beginning of the contents of the input buffer
        static constexpr unsigned int OFFSET_INPUT_DATA_END = OFFSET_INPUT_BEGIN + BUFFER_SIZE;         // The end of the contents of the input buffer

        static constexpr unsigned int OFFSET_OUTPUT_BEGIN = OFFSET_INPUT_DATA_END;                      // The iterator to the beginning of the output buffer
        static constexpr unsigned int OFFSET_OUTPUT_END = OFFSET_OUTPUT_BEGIN + 1;                      // The iterator to the end of the output buffer
        static constexpr unsigned int OFFSET_OUTPUT_DATA_BEGIN = OFFSET_OUTPUT_END + 1;                 // The beginning of the contents of the output buffer
        static constexpr unsigned int OFFSET_OUTPUT_DATA_END = OFFSET_OUTPUT_DATA_BEGIN + BUFFER_SIZE;  // The end of the contents of the output buffer
};

#endif /* end of include guard: PRUDEVICE_H_GZ1JA6SL */
