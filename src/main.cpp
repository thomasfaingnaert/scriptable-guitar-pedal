#include <cstdlib>
#include <iostream>

#include "sampledata.h"
#include "NE10.h"
#include "filesource.h"
#include "filesink.h"
#include "filtereffect.h"
#include "webserver.h"

int main(int argc, char *argv[])
{
    // NE10 Initialisation
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    // Webserver
    WebServer server(8888);

    std::cout << "Webserver is running" << std::endl;

    while(server.isRunning());

    std::cout << "Webserver shutdown" << std::endl;


    return EXIT_SUCCESS;
}

