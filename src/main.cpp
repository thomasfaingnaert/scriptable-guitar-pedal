#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>

#include "NE10.h"
#include "adder.h"
#include "civetweb.h"
#include "delayeffect.h"
#include "distortioneffect.h"
#include "filesink.h"
#include "filesource.h"
#include "processor.h"
#include "sinesource.h"
#include "sink.h"
#include "source.h"
#include "streamsink.h"
#include "tremoloeffect.h"
#include "webserver.h"

int main(int argc, char *argv[])
{
    // NE10 Initialisation
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    for (int i = 1; i <= 20; ++i)
    {
        const unsigned int n = std::pow(2, i);
        long long duration = 0;
        for (int j = 0; j < 1000; ++j)
        {
            std::vector<float> samples(n);
            std::vector<ne10_fft_cpx_float32_t> output(n);

            ne10_fft_r2c_cfg_float32_t cfg = ne10_fft_alloc_r2c_float32(n);

            for (int i = 0; i < n; ++i)
            {
                samples[i] = rand() / RAND_MAX * 1.0f;
            }

            auto begin = std::chrono::high_resolution_clock::now();

            //while (in.generate_next()) ;
            ne10_fft_r2c_1d_float32(output.data(), samples.data(), cfg);

            auto end = std::chrono::high_resolution_clock::now();
            duration += std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
        }

        std::cout << n << ": took " << duration / (n * 1000) << " ns/sample\n";
    }

    return EXIT_SUCCESS;
}
