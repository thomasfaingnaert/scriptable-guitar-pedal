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
#include "filesink.h"
#include "filesource.h"
#include "filtereffect.h"

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
        //std::this_thread::sleep_until(begin + std::chrono::microseconds(5333));
    }

    snk->write();

    std::cout << "min: " << std::chrono::duration_cast<std::chrono::microseconds>(min).count() << " us\n"
                 "max: " << std::chrono::duration_cast<std::chrono::microseconds>(max).count() << " us\n"
                 "avg: " << std::chrono::duration_cast<std::chrono::microseconds>(total/count).count() << " us\n";


    return EXIT_SUCCESS;
}

