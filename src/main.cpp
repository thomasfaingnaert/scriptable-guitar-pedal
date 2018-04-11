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
#include "fftconvolver.h"
#include "sampledata.h"

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

float timeFFT(const unsigned int impulseSize, const unsigned int blockSize, const unsigned int numBlocks)
{
    std::vector<float> impulseResponse(impulseSize);
    std::generate(impulseResponse.begin(), impulseResponse.end(), std::rand);

    FFTConvolver fftConvolver(impulseResponse, blockSize);

    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned int i = 0; i < numBlocks; ++i)
    {
        std::vector<float> input(blockSize);
        std::generate(input.begin(), input.end(), std::rand);
        std::vector<float> result = fftConvolver.process(input);
    }
    auto end = std::chrono::high_resolution_clock::now();
    const auto executionTime = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    const unsigned long long inputTime = numBlocks * blockSize / 48;

    // calculate processor load
    return 100.0 * executionTime / inputTime;
}

void testFFTConv()
{
    unsigned int blockSize = 16;
    unsigned int impulseLength = 1024;

    std::vector<float> impulse(impulseLength);
    std::generate(impulse.begin(), impulse.end(), std::rand);

    FFTConvolver conv(impulse, blockSize);
    DirectFormConvolver conv2(impulse, blockSize);

    for (int i = 0; i < 10; ++i)
    {
        std::vector<float> input(blockSize);
        std::generate(input.begin(), input.end(), std::rand);

        std::vector<float> result = conv.process(input);
        std::vector<float> result2 = conv2.process(input);

        std::cout << "Block " << i << ":\n";
        std::copy(result.begin(), result.end(), std::ostream_iterator<float>(std::cout, " "));
        std::cout << "\n";
        std::copy(result2.begin(), result2.end(), std::ostream_iterator<float>(std::cout, " "));
        std::cout << "\n\n\n";
    }
}

int main(int argc, char *argv[])
{
    // NE10 Initialisation
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    //testFFTConv();

    SampleData input("input.wav");
    SampleData impulse("impulse.wav");

    std::vector<float> inputSamples = input.getSamples()[0];
    std::vector<float> impulseSamples = impulse.getSamples()[0];

    std::vector<float> result;

    unsigned int blockSize = impulseSamples.size() * 2;
    FFTConvolver conv(impulseSamples, blockSize);

    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i + blockSize < inputSamples.size(); i += blockSize)
    {
        std::vector<float> block(inputSamples.begin() + i, inputSamples.begin() + i + blockSize);
        std::vector<float> res = conv.process(block);
        result.insert(result.end(), res.begin(), res.end());
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " ms" << "\n";

    float max = 0;

    for (float r : result)
        max = std::max(max, std::abs(r));

    for (float& r : result)
        r /= max;

    SampleData output({result}, input.getSampleRate());
    output.save("output.wav");

#if 0
    constexpr unsigned int FIELD_WIDTH = 30;
    constexpr unsigned int NUM_TRIALS = 10;

    const std::vector<float> impulseDurations = {0.01, 0.02, 0.05, 0.1, 0.2};

    for (float impulseDuration : impulseDurations)
    {
        unsigned int impulseSize = 48000 * impulseDuration;

        std::cout << std::left;
        std::cout << std::setw(FIELD_WIDTH) << "Type: " << "FFT (Neon)" << "\n";
        std::cout << std::setw(FIELD_WIDTH) << "Impulse length: " << (float)impulseSize / 48 << " ms\n";
        std::cout << std::setw(FIELD_WIDTH) << "#Trials per blocksize: " << NUM_TRIALS << "\n";
        std::cout << std::setw(FIELD_WIDTH) << "Blocksize (samples)" << std::setw(FIELD_WIDTH) << "Load (%)" << "\n";

        for (unsigned int blockSize = 8; blockSize <= 32000; blockSize *= 2)
        {
            unsigned int numBlocks = 10 * 48000 / blockSize; // input = 10 s
            float load = 0.0;

            for (unsigned int i = 0; i < NUM_TRIALS; ++i)
            {
                load += timeFFT(impulseSize, blockSize, numBlocks);
            }

            std::cout << std::setw(FIELD_WIDTH) << blockSize << std::setw(FIELD_WIDTH) << load / NUM_TRIALS << std::endl;
        }
    }
#endif

    return EXIT_SUCCESS;
}

