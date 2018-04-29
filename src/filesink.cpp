#include <stdexcept>

#include "constants.h"
#include "filesink.h"
#include "sampledata.h"

FileSink::FileSink(const std::string& filename, unsigned int samplerate)
    : filename(filename), samplerate(samplerate)
{
}

void FileSink::push(const std::array<float, Constants::BLOCK_SIZE>& data)
{
    samples.insert(samples.end(), data.begin(), data.end());
}

void FileSink::write()
{
    std::vector<float> samplesVector(samples.begin(), samples.end());
    SampleData data({ samplesVector }, samplerate);
    data.save(filename);
}
