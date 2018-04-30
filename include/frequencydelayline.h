#ifndef FREQUENCYDELAYLINE_H_HJRFQLZO
#define FREQUENCYDELAYLINE_H_HJRFQLZO

#include <algorithm>
#include <array>
#include <vector>
#include "NE10.h"
#include "blockbuffer.h"
#include "arm_neon_complex.h"

class FrequencyDelayLine
{
    public:
        typedef ne10_fft_cpx_float32_t Complex;
        FrequencyDelayLine(unsigned int blockSize, const std::vector<std::vector<float>>& impulseResponses);
        ~FrequencyDelayLine();

        template<typename InputIt, typename OutputIt>
        void process(InputIt in_begin, InputIt in_end, OutputIt out_begin)
        {
            // Add new input block
            inputBuffer.push_back(in_begin, in_end);

            // First calculate the FFT of the next input segment (= new block + overlap from last block)
            ne10_fft_r2c_1d_float32(inputFourier.data(), inputBuffer.begin(), config);

            // Add this new FFT to the buffer for frequency data
            frequencyBuffer.push_back(inputFourier.begin(), inputFourier.end());

            // Remember position in frequencyBuffer
            float* inputFreqIterator = (float*) frequencyBuffer.data();

            // Use normal multiply for first convolver
            if (!frequencyResponses.empty())
            {
                arm_neon_complex_multiply((float*) outputFourier.data(), (float*) frequencyResponses[frequencyResponses.size() - 1].data(), (float*) inputFreqIterator, COMPLEX_SIZE);
                inputFreqIterator += 2 * COMPLEX_SIZE;
            }

            // Use multiply-accumulate for rest
            for (int i = frequencyResponses.size() - 2; i >= 0; --i)
            {
                arm_neon_complex_multiply_accumulate((float*) outputFourier.data(), (float*) outputFourier.data(), (float*) frequencyResponses[i].data(), (float*) inputFreqIterator, COMPLEX_SIZE);
                inputFreqIterator += 2 * COMPLEX_SIZE;
            }

            // Inverse transform to get output
            ne10_fft_c2r_1d_float32(output.data(), outputFourier.data(), config);

            // Remove the overlap at the beginning of output
            std::copy_n(output.begin() + OVERLAP, N, out_begin);
        }

    private:
        const unsigned int N; // blockSize
        const unsigned int PERIOD; // 2 * N
        const unsigned int OVERLAP; // N
        const unsigned int COMPLEX_SIZE; // PERIOD / 2 + 8

        BlockBuffer<float> inputBuffer;
        BlockBuffer<Complex> frequencyBuffer;

        ne10_fft_r2c_cfg_float32_t config;
        std::vector<std::vector<Complex>> frequencyResponses;

        // Buffers used in process()
        std::vector<Complex> inputFourier; // size = COMPLEX_SIZE
        std::vector<Complex> outputFourier; // size = COMPLEX_SIZE
        std::vector<float> output; // size = PERIOD

        // Only use powers of two as blocksizes
        constexpr static bool isPowerOfTwo(unsigned int x) { return x && ((x & (x - 1)) == 0); }
};

#endif /* end of include guard: FREQUENCYDELAYLINE_H_HJRFQLZO */
