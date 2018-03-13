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
#include "convolver.h"
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

    /* Test convolver */
    std::vector<float> impulse(Source<float>::BLOCK_SIZE);
    std::iota(impulse.begin(), impulse.end(), 1);
    Convolver conv(impulse);

    std::cout << "Block size: " << Source<float>::BLOCK_SIZE << "\n";

    auto begin = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 10000; ++i)
    {
        auto block = std::make_shared<std::vector<float>>(Source<float>::BLOCK_SIZE);
        std::iota(block->begin(), block->end(), 1 + Source<float>::BLOCK_SIZE * i);
        auto result = conv.process({ block });
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "took " << std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count() << " us\n";

    return EXIT_SUCCESS;
}
