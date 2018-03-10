#include <string>

#include "source.h"

class FileSource : public Source<float>
{
    public:
        FileSource(const std::string& filename);
        bool generate_next();

    private:
        std::vector<float> samples;
        std::vector<float>::iterator currentSample;
};
