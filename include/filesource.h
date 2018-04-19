#include <string>

#include <vector>

#include "source.h"

class FileSource : public Source<float>
{
    public:
        FileSource(const std::string& filename);
        bool generate_next();
        unsigned int getSampleRate() const { return sampleRate; }

    private:
        std::vector<float> samples;
        std::vector<float>::iterator currentSample;
        unsigned int sampleRate;
};
