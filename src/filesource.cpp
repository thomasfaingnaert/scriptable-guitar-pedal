#include <algorithm>

#include "filesource.h"
#include "sampledata.h"

FileSource::FileSource(const std::string& filename)
{
    SampleData data(filename);
    samples = data.getSamples()[0];
    currentSample = samples.begin();
    sampleRate = data.getSampleRate();
}

bool FileSource::generate_next()
{
    if (currentSample == samples.end())
        return false;

    if (std::distance(currentSample, samples.end()) >= static_cast<std::vector<float>::iterator::difference_type>(Constants::BLOCK_SIZE))
    {
        std::array<float, Constants::BLOCK_SIZE> block;
        std::copy_n(currentSample, Constants::BLOCK_SIZE, block.begin());
        generate(block);
        currentSample += Constants::BLOCK_SIZE;
        return true;
    }
    else
    {
        std::array<float, Constants::BLOCK_SIZE> block;
        std::copy(currentSample, samples.end(), block.begin());
        generate(block);
        currentSample = samples.end();
        return false;
    }
}
