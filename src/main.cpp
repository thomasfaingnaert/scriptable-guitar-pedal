#include <cstdlib>
#include <iostream>

#include "NE10.h"
#include "webserver.h"

int main(int argc, char *argv[])
{
    // NE10 Initialisation
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    WebServer server(8888);

    std::cout << "Server running" << std::endl;

    while(server.isRunning());

    return EXIT_SUCCESS;
}

