#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <thread>

#include "NE10.h"
#include "codec.h"
#include "filesink.h"
#include "filesource.h"
#include "filtereffect.h"
#include "prudevice.h"
#include "webserver.h"

int main(int argc, char *argv[])
{
    // NE10 Initialisation
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    // Set thread priority
    sched_param param;
    param.sched_priority = 99;

    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &param) != 0)
    {
        std::cerr << "Failed to set priority for thread: Error code " << errno << ": " << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    // Webserver
    WebServer server(8888);
    while(server.isRunning());

    return EXIT_SUCCESS;
}

