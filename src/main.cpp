#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>

#include "NE10.h"
#include "adder.h"
#include "blockbuffer.h"
#include "civetweb.h"
#include "codec.h"
#include "convolver.h"
#include "delayeffect.h"
#include "distortioneffect.h"
#include "filesink.h"
#include "filesource.h"
#include "processor.h"
#include "rapidjson/document.h"
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

    // RapidJson test
    const char* json = " { \"hello\": \"world\", \"name\": \"Robbe\" }";
    rapidjson::Document document;
    document.Parse(json);
    std::cout << "hello = " << document["hello"].GetString() << std::endl;

    WebServer server(8888);

    std::cout << "Server running" << std::endl;

    while (server.isRunning());

    return EXIT_SUCCESS;
}
