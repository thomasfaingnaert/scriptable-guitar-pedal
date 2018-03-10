#include <cstdlib>
#include <iostream>
#include <string>
#include <iterator>

#include "NE10.h"
#include "civetweb.h"
#include "webserver.h"
#include "samplebuffer.h"

int main(int argc, char *argv[])
{
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    mg_init_library(0);
    WebServer server(8888);
    while (server.isRunning()) ;
    mg_exit_library();


    return EXIT_SUCCESS;
}
