#include <stdexcept>

#include "filesink.h"
#include "sampledata.h"

void FileSink::push(const std::shared_ptr<std::vector<float>>& t, unsigned int channel)
{
    if (channel != 0)
        throw std::invalid_argument("Channel of FileSink must be zero");
    samples.insert(samples.end(), t->begin(), t->end());
}

void FileSink::write()
{
    SampleData data({ samples }, 48000);
    data.save(filename);
}
