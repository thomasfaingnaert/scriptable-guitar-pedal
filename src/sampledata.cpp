#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <vector>

#include "sampledata.h"
#include "sndfile.hh"

SampleData::SampleData(const std::string& filename)
{
    load(filename);
}

SampleData::SampleData(const std::vector<std::vector<Sample>> samples, unsigned int rate) : sampleRate(rate)
{
    numChannels = samples.size();

    if (!samples.empty())
    {
        sampleData.reserve(samples.size() * samples[0].size());
        for (size_t j = 0; j < samples[0].size(); ++j)
        {
            for (size_t i = 0; i < samples.size(); ++i)
            {
                sampleData.push_back(samples[i][j]);
            }
        }
    }
}

unsigned int SampleData::getSampleRate() const
{
    return sampleRate;
}

unsigned int SampleData::getNumChannels() const
{
    return numChannels;
}

unsigned int SampleData::getNumFrames() const
{
    return sampleData.size() / numChannels;
}

Sample SampleData::get(size_t channel, size_t index) const
{
    return sampleData.at(index * numChannels + channel);
}

void SampleData::set(size_t channel, size_t index, Sample sample)
{
    sampleData.at(index * numChannels + channel) = sample;
}

void SampleData::load(const std::string& filename)
{
    // open file for reading
    SndfileHandle file(filename);
    sampleRate = file.samplerate();
    numChannels = file.channels();

    // clear old data
    sampleData.clear();

    // read buffer
    std::vector<Sample> buffer(BUFFER_SIZE * numChannels);

    // read block-per-block
    unsigned int numItemsRead;
    while ( (numItemsRead = file.read(buffer.data(), buffer.size())) > 0)
    {
        // reserve enough space for the new block to avoid reallocation
        sampleData.reserve(sampleData.size() + numItemsRead);

        // add new block to sampleData
        std::copy_n(buffer.begin(), numItemsRead, std::back_inserter(sampleData));
    }
}

void SampleData::save(const std::string& filename, bool normalize) const
{
    std::vector<float> samples = sampleData;
    if (normalize)
    {
        float max = 0.0;
        for (float f : samples)
            max = std::max(max, std::abs(f));
        for (float& f : samples)
            f /= max;
    }

    // open file for writing
    SndfileHandle file(filename, SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_PCM_16, numChannels, sampleRate);
    // write all samples
    file.write(samples.data(), samples.size());
}

std::vector<std::vector<Sample>> SampleData::getSamples() const
{
    std::vector<std::vector<Sample>> samples(numChannels, std::vector<Sample>());

    for (size_t i = 0; i < numChannels; ++i)
    {
        for (size_t j = 0; j < getNumFrames(); ++j)
        {
            samples[i].push_back(get(i, j));
        }
    }

    return samples;
}
