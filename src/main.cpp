#include <algorithm>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <pthread.h>
#include <thread>
#include <vector>

#include "NE10.h"
#include "sampledata.h"
#include "filesink.h"
#include "filesource.h"
#include "filtereffect.h"
#include "frequencydelayline.h"

int main(int argc, char *argv[])
{
    // NE10 Initialisation
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    sched_param param;
    param.sched_priority = 99;
    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &param) != 0)
    {
        std::cerr << "Failed to set priority for thread: Error code " << errno << ": " << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

#if 1
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
    out.save("output.wav", true);
    std::cout << "Saved" << std::endl;
#endif

#if 0
    auto src = std::make_shared<FileSource>("input.wav");
    auto eff = std::make_shared<FilterEffect>();
    auto snk = std::make_shared<FileSink>("output.wav", src->getSampleRate());

    src->connect(eff);
    eff->connect(snk);

    bool cont = true;
    unsigned int count = 0;
    std::chrono::duration<double> total, min = std::chrono::duration<double>::max(), max = std::chrono::duration<double>::min();
    while (cont)
    {
        auto begin = std::chrono::high_resolution_clock::now();
        cont = src->generate_next();
        auto duration = std::chrono::high_resolution_clock::now() - begin;

        total += duration;
        min = duration < min ? duration : min;
        max = duration > max ? duration : max;
        ++count;
        std::this_thread::sleep_until(begin + std::chrono::microseconds(5333));
    }

    snk->write();

    std::cout << "min: " << std::chrono::duration_cast<std::chrono::microseconds>(min).count() << " us\n"
                 "max: " << std::chrono::duration_cast<std::chrono::microseconds>(max).count() << " us\n"
                 "avg: " << std::chrono::duration_cast<std::chrono::microseconds>(total/count).count() << " us\n";


#endif
    return EXIT_SUCCESS;
}

