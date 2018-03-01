#include <cstdlib>
#include <iostream>

#include "sampledata.h"

int main(int argc, char *argv[])
{
    SampleData data("dry-guitar-cut.wav");

    std::cout << "channels: " << data.getNumChannels() << "\n";
    std::cout << "samplerate: " << data.getSampleRate() << "\n";
    std::cout << "frames: " << data.getNumFrames() << "\n";

    std::cout << "samples:\n";
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < data.getNumChannels(); ++j)
        {
            std::cout << data.get(j, i) << ' ';
        }
        std::cout << '\n';
    }

    data.save("out.wav");

    return EXIT_SUCCESS;
}
