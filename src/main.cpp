#include <cstdlib>
#include <iostream>

#include "sampledata.h"
#include "NE10.h"
#include "filesource.h"
#include "filesink.h"
#include "filtereffect.h"

int main(int argc, char *argv[])
{
    // NE10 Initialisation
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    SampleData impulse("impulse.wav");
    auto impulseSamples = impulse.getSamples()[0];

    auto source = std::make_shared<FileSource>("input.wav");
    auto effect = std::make_shared<FilterEffect>(impulseSamples);
    auto sink = std::make_shared<FileSink>("output.wav", source->getSampleRate());

    source->connect(effect);
    effect->connect(sink);

    while (source->generate_next()) ;

    sink->write();

    return EXIT_SUCCESS;
}

