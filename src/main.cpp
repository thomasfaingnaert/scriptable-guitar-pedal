#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <numeric>

#include "sndfile.hh"
#include "sampledata.h"

void write()
{
    std::cout << "Writing file...\n";
    static constexpr size_t BUFFER_LENGTH = 1024;
    std::array<double, BUFFER_LENGTH> buffer;

    std::iota(buffer.begin(), buffer.end(), 0);
    std::copy(buffer.begin(), buffer.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";

    SndfileHandle file("test.wav", SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_24, 2, 48000);
    file.write(buffer.data(), buffer.size());
}

void read()
{
    std::cout << "Reading file...\n";
    static constexpr size_t BUFFER_LENGTH = 1024;
    std::array<double, BUFFER_LENGTH> buffer;

    SndfileHandle file("test.wav");

    std::cout << "Sample rate: " << file.samplerate() << "\n"
        "Channels: " << file.channels() << "\n";

    file.read(buffer.data(), buffer.size());

    std::copy(buffer.begin(), buffer.end(), std::ostream_iterator<double>(std::cout, " "));
    std::cout << "\n";
}

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
