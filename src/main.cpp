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
#include "delayeffect.h"
#include "distortioneffect.h"
#include "filesink.h"
#include "filesource.h"
#include "processor.h"
#include "sinesource.h"
#include "sink.h"
#include "source.h"
#include "streamsink.h"
#include "tremoloeffect.h"
#include "webserver.h"

int main(int argc, char *argv[])
{
    // NE10 Initialisation
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    // Test effects
    FileSource in("input.wav");
    auto eff = std::make_shared<DelayEffect>(DelayEffect(1.0f, { static_cast<unsigned int>(44100 * 0.2 / in.BLOCK_SIZE) }, { 0.2f }));
    auto out = std::make_shared<FileSink>("output.wav");

    in.connect(eff, 0);
    eff->connect(out, 0);

    auto begin = std::chrono::high_resolution_clock::now();

    while (in.generate_next()) ;

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "took " << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() << " ms\n";

    out->write();

    /*
    // Start web server
    mg_init_library(0);
    WebServer server(8888);
    while (server.isRunning()) ;
    mg_exit_library();
    */


    return EXIT_SUCCESS;
}
