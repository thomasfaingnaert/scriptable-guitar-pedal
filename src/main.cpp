#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <vector>

#include "NE10.h"
#include "adder.h"
#include "alsadevice.h"
#include "blockbuffer.h"
#include "civetweb.h"
#include "codec.h"
#include "convolver.h"
#include "delayeffect.h"
#include "distortioneffect.h"
#include "filesink.h"
#include "filesource.h"
#include "luaeffect.h"
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

    auto alsaDevice = std::make_shared<AlsaDevice>(1, 0, 48000, 2, 2, 1024, 1024, 2, 2);

    auto fileSink = std::make_shared<FileSink>("input.wav");

    alsaDevice->connect(fileSink, 0);

    for (int i = 0; i < 3750; i++)
        alsaDevice->generate_next();

    fileSink->write();

    return EXIT_SUCCESS;
}

