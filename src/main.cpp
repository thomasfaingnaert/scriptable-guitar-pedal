#include <cstdlib>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>

#include "NE10.h"
#include "civetweb.h"
#include "webserver.h"
#include "samplebuffer.h"
#include "source.h"
#include "sink.h"

int main(int argc, char *argv[])
{
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    Source<int> src;
    std::shared_ptr<Sink<int>> sink = std::make_shared<Sink<int>>();

    src.connect(sink, 0);
    src.connect(sink, 1);

    for (int i = 1; i <= 10; ++i)
    {
        src.generate(i);
    }

    /*
    mg_init_library(0);
    WebServer server(8888);
    while (server.isRunning()) ;
    mg_exit_library();
    */


    return EXIT_SUCCESS;
}
