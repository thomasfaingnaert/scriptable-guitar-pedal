#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <vector>

#include "frequencydelayline.h"
#include "sampledata.h"

int main(int argc, char *argv[])
{
    // NE10 Initialisation
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    constexpr unsigned int BLOCK_SIZE = 1024 * 128;

    std::cout << "Block size: " << BLOCK_SIZE << std::endl;

    std::cout << "Loading wavs..." << std::endl;
    SampleData input("input.wav");
    SampleData impulse("impulse.wav");

    auto inputSamples = input.getSamples()[0];
    auto impulseSamples = impulse.getSamples()[0];

    std::cout << "Partitioning impulse..." << std::endl;
    std::vector<std::array<float,BLOCK_SIZE+1>> impulseResponses;

    auto it = impulseSamples.begin();

    while (it != impulseSamples.end())
    {
        impulseResponses.emplace_back();

        if (std::distance(it, impulseSamples.end()) < BLOCK_SIZE+1)
        {
            std::copy(it, impulseSamples.end(), impulseResponses.back().begin());
            it = impulseSamples.end();
        }
        else
        {
            std::copy_n(it, BLOCK_SIZE+1, impulseResponses.back().begin());
            it += BLOCK_SIZE + 1;
        }
    }

    std::cout << impulseResponses.size() << " partitions" << std::endl;

    std::cout << "Creating fdl..." << std::endl;
    FrequencyDelayLine<BLOCK_SIZE> fdl(impulseResponses);

    auto inIt = inputSamples.begin();
    std::vector<float> outputSamples;

    std::cout << "Processing..." << std::endl;
    auto begin = std::chrono::high_resolution_clock::now();
    while (inIt + BLOCK_SIZE < inputSamples.end())
    {
        std::array<float, BLOCK_SIZE> block;
        std::copy_n(inIt, BLOCK_SIZE, block.begin());

        auto res = fdl.process(block);
        outputSamples.insert(outputSamples.end(), res.begin(), res.end());
        inIt += BLOCK_SIZE;
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Processing took " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " ms" << std::endl;

    std::cout << "Saving..." << std::endl;
    SampleData out({outputSamples}, input.getSampleRate());
    out.save("output.wav");
    std::cout << "Saved" << std::endl;

    return EXIT_SUCCESS;
}

