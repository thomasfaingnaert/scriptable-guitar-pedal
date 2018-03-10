#include <cstdlib>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>

#include "NE10.h"
#include "adder.h"
#include "civetweb.h"
#include "processor.h"
#include "samplebuffer.h"
#include "sink.h"
#include "source.h"
#include "streamsink.h"
#include "webserver.h"

int main(int argc, char *argv[])
{
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    Source<float> src1, src2;
    std::shared_ptr<Adder> adder = std::make_shared<Adder>();
    std::shared_ptr<Sink<float>> output = std::make_shared<StreamSink<float>>(std::cout);

    src1.connect(adder, 0);
    src2.connect(adder, 1);
    adder->connect(output, 0);

    for (int i = 1; i <= Source<float>::BLOCK_SIZE; ++i)
    {
        src1.generate(i);
        src2.generate(0);
    }

    /*
    mg_init_library(0);
    WebServer server(8888);
    while (server.isRunning()) ;
    mg_exit_library();
    */


    return EXIT_SUCCESS;
}
