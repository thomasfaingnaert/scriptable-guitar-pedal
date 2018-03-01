#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <numeric>

#include "sndfile.hh"

void write()
{
    std::cout << "Writing file...\n";
    static constexpr size_t BUFFER_LENGTH = 1024;
    std::array<short, BUFFER_LENGTH> buffer;

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
    std::array<short, BUFFER_LENGTH> buffer;

    SndfileHandle file("test.wav");

    std::cout << "Sample rate: " << file.samplerate() << "\n"
        "Channels: " << file.channels() << "\n";

    file.read(buffer.data(), buffer.size());

    std::copy(buffer.begin(), buffer.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
}

int main(int argc, char *argv[])
{
    write();
    read();

    return EXIT_SUCCESS;
}
