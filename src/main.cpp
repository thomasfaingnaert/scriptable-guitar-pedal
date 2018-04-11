#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <vector>

#include "NE10.h"
#include "directformconvolver.h"

float timeDirectForm(const unsigned int impulseSize, const unsigned int blockSize, const unsigned int numBlocks)
{
    std::vector<float> impulseResponse(impulseSize);
    std::generate(impulseResponse.begin(), impulseResponse.end(), std::rand);

    DirectFormConvolver directFormConvolver(impulseResponse, blockSize);

    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned int i = 0; i < numBlocks; ++i)
    {
        std::vector<float> input(blockSize);
        std::generate(input.begin(), input.end(), std::rand);
        std::vector<float> result = directFormConvolver.process(input);
    }
    auto end = std::chrono::high_resolution_clock::now();
    const auto executionTime = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    const unsigned long long inputTime = numBlocks * blockSize / 48;

    // calculate processor load
    return 100.0 * executionTime / inputTime;
}

int main(int argc, char *argv[])
{
    // NE10 Initialisation
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    constexpr unsigned int FIELD_WIDTH = 30;
    constexpr unsigned int NUM_TRIALS = 10;


    const std::vector<float> impulseDurations = {0.01, 0.02, 0.05, 0.1, 0.2};

    for (float impulseDuration : impulseDurations)
    {
        unsigned int impulseSize = 48000 * impulseDuration;

        std::cout << std::left;
        std::cout << std::setw(FIELD_WIDTH) << "Type: " << "Direct Form (Neon)" << "\n";
        std::cout << std::setw(FIELD_WIDTH) << "Impulse length: " << (float)impulseSize / 48 << " ms\n";
        std::cout << std::setw(FIELD_WIDTH) << "#Trials per blocksize: " << NUM_TRIALS << "\n";
        std::cout << std::setw(FIELD_WIDTH) << "Blocksize (samples)" << std::setw(FIELD_WIDTH) << "Load (%)" << "\n";

        for (unsigned int blockSize = 8; blockSize <= 32000; blockSize *= 2)
        {
            unsigned int numBlocks = 10 * 48000 / blockSize; // input = 10 s
            float load = 0.0;

            for (unsigned int i = 0; i < NUM_TRIALS; ++i)
            {
                load += timeDirectForm(impulseSize, blockSize, numBlocks);
            }

            std::cout << std::setw(FIELD_WIDTH) << blockSize << std::setw(FIELD_WIDTH) << load / NUM_TRIALS << std::endl;
        }
    }

    return EXIT_SUCCESS;
}

