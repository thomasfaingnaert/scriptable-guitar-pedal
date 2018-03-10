#include "filesource.h"
#include "sampledata.h"

FileSource::FileSource(const std::string& filename)
{
    SampleData data(filename);
    samples = data.getSamples()[0];
    currentSample = samples.begin();
}

bool FileSource::generate_next()
{
    if (currentSample == samples.end())
        return false;

    if (std::distance(currentSample, samples.end()) >= BLOCK_SIZE)
    {
        auto block = std::make_shared<std::vector<float>>(currentSample, currentSample + BLOCK_SIZE);
        generate(block);
        currentSample += BLOCK_SIZE;
        return true;
    }
    else
    {
        auto block = std::make_shared<std::vector<float>>(currentSample, samples.end());
        block->resize(BLOCK_SIZE);
        generate(block);
        currentSample = samples.end();
        return false;
    }
}
