#include <cstdlib>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>

#include "NE10.h"
#include "adder.h"
#include "civetweb.h"
#include "processor.h"
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

    Source<float> src1;
    Source<float> src2;

    auto adder = std::make_shared<Adder>();
    auto out = std::make_shared<StreamSink<float>>(std::cout);

    auto data1 = std::make_shared<std::vector<float>>();
    data1->resize(Source<float>::BLOCK_SIZE);
    std::iota(data1->begin(), data1->end(), 1);

    auto data2 = std::make_shared<std::vector<float>>();
    data2->resize(Source<float>::BLOCK_SIZE);
    std::fill(data2->begin(), data2->end(), 1);

    src1.connect(adder, 0);
    src2.connect(adder, 1);
    adder->connect(out, 0);
    src1.connect(out, 1);
    src2.connect(out, 2);

    src1.generate(data1);
    src2.generate(data2);

    /*
    mg_init_library(0);
    WebServer server(8888);
    while (server.isRunning()) ;
    mg_exit_library();
    */


    return EXIT_SUCCESS;
}
