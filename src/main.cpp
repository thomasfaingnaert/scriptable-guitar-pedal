#include <chrono>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>

#include "NE10.h"
#include "adder.h"
#include "civetweb.h"
#include "filesink.h"
#include "filesource.h"
#include "processor.h"
#include "sinesource.h"
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

    SineSource src(1.0, 44100 / 440);
    auto outputFile = std::make_shared<FileSink>("output.wav");

    src.connect(outputFile, 0);

    auto begin = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 44100 * 30 / src.BLOCK_SIZE; ++i)
        src.generate_next();

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "took " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " ms\n";

    outputFile->write();

    /*
    mg_init_library(0);
    WebServer server(8888);
    while (server.isRunning()) ;
    mg_exit_library();
    */


    return EXIT_SUCCESS;
}
