#ifndef FREQUENCYDELAYLINE_H_HJRFQLZO
#define FREQUENCYDELAYLINE_H_HJRFQLZO

#include <algorithm>
#include <array>
#include <vector>
#include "NE10.h"
#include "blockbuffer.h"
#include "arm_neon_complex.h"

template<unsigned int N>
class FrequencyDelayLine
{
    public:
        typedef ne10_fft_cpx_float32_t Complex;

        FrequencyDelayLine(const std::vector<std::array<float, N+1>>& impulseResponses)
            : inputBuffer(2, N), frequencyBuffer(impulseResponses.size(), COMPLEX_SIZE)
        {
            // Allocate configuration for Ne10 FFT
            config = ne10_fft_alloc_r2c_float32(PERIOD);

            for (const auto& impulseResponse : impulseResponses)
            {
                // Resize impulseResponses to PERIOD samples by adding zeros
                std::array<float, PERIOD> impulseResized{};
                std::copy(impulseResponse.begin(), impulseResponse.end(), impulseResized.begin());

                // Calculate frequency responses
                frequencyResponses.emplace_back();
                ne10_fft_r2c_1d_float32(frequencyResponses.back().data(), impulseResized.data(), config);
            }
        }

        ~FrequencyDelayLine()
        {
            ne10_fft_destroy_r2c_float32(config);
        }

        std::array<float, N> process(const std::array<float, N>& input)
        {
            // Add new input block
            inputBuffer.push_back(input.begin(), input.end());

            // First calculate the FFT of the next input segment (= new block + overlap from last block)
            std::array<Complex, COMPLEX_SIZE> inputFourier;
            ne10_fft_r2c_1d_float32(inputFourier.data(), inputBuffer.begin(), config);

            // Add this new FFT to the buffer for frequency data
            frequencyBuffer.push_back(inputFourier.begin(), inputFourier.end());


            // Iterate through all convolvers
            std::array<Complex, COMPLEX_SIZE> outputFourier{};
            float* inputFreqIterator = (float*) frequencyBuffer.data();

            for (int i = frequencyResponses.size() - 1; i >= 0; --i)
            {
                // Calculate output for this convolver
                // TODO: Fix complex multiply not working if size % 8 != 0
                static_assert(COMPLEX_SIZE % 8 == 0, "Complex MAC does not work if COMPLEX_SIZE is not divisible by 8");
                arm_neon_complex_multiply_accumulate((float*) outputFourier.data(), (float*) outputFourier.data(), (float*) frequencyResponses[i].data(), (float*) inputFreqIterator, COMPLEX_SIZE);
                inputFreqIterator += 2 * COMPLEX_SIZE;
            }

            // Inverse transform to get output
            std::array<float, PERIOD> output;
            ne10_fft_c2r_1d_float32(output.data(), outputFourier.data(), config);

            // Remove the overlap at the beginning of output
            std::array<float, N> result;
            std::copy_n(output.begin() + OVERLAP, N, result.begin());
            return result;
        }

    private:

        constexpr static unsigned int PERIOD = 2*N;
        constexpr static unsigned int OVERLAP = N;
        constexpr static unsigned int COMPLEX_SIZE = PERIOD / 2 + 8;

        BlockBuffer<float> inputBuffer;
        BlockBuffer<Complex> frequencyBuffer;

        ne10_fft_r2c_cfg_float32_t config;
        std::vector<std::array<Complex, COMPLEX_SIZE>> frequencyResponses;

        // Only use powers of two as blocksizes
        constexpr static bool isPowerOfTwo(unsigned int x) { return x && ((x & (x - 1)) == 0); }
        static_assert(isPowerOfTwo(N), "Blocksize of FDL should be power of two");
};

#endif /* end of include guard: FREQUENCYDELAYLINE_H_HJRFQLZO */
