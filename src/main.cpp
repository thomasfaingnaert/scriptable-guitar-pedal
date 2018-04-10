#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <vector>

#include "NE10.h"
#include "directformconvolver.h"

unsigned long long timeDirectForm(const unsigned int impulseSize, const unsigned int blockSize, const unsigned int numBlocks)
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
    return std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
}

int main(int argc, char *argv[])
{
    // NE10 Initialisation
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    unsigned int IMPULSE_SIZE = 48000 * 0.01;
    unsigned int BLOCK_SIZE;
    std::cout << "Blocksize (samples): ";
    std::cin >> BLOCK_SIZE;
    unsigned int inputSecs;
    std::cout << "Input length (s): ";
    std::cin >> inputSecs;
    unsigned int NUM_BLOCKS = inputSecs * 48000 / BLOCK_SIZE;

    unsigned long long executionTime = timeDirectForm(IMPULSE_SIZE, BLOCK_SIZE, NUM_BLOCKS);
    unsigned long long inputTime = NUM_BLOCKS * BLOCK_SIZE / 48;

    std::cout << "Blocksize: " << BLOCK_SIZE << "\n";
    std::cout << "Processing took " << executionTime << " ms\n";
    std::cout << "Input is " << inputTime << " ms long\n";
    std::cout << "Processor load is " << 100.0 * executionTime / inputTime << "%\n";

    return EXIT_SUCCESS;
}

