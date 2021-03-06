#include <cstddef>
#include <string>
#include <vector>

#ifndef SAMPLEDATA_H_FIPJCQHI
#define SAMPLEDATA_H_FIPJCQHI

typedef float Sample;

class SampleData
{
    public:
        SampleData(const std::string& filename);
        SampleData(const std::vector<std::vector<Sample>> samples, unsigned int rate);

        unsigned int getSampleRate() const;
        unsigned int getNumChannels() const;
        unsigned int getNumFrames() const;

        void load(const std::string& filename);
        void save(const std::string& filename, bool normalize = false) const;

        Sample get(size_t channel, size_t index) const;
        void set(size_t channel, size_t index, Sample sample);

        std::vector<std::vector<Sample>> getSamples() const;

    private:
        std::vector<Sample> sampleData;
        unsigned int sampleRate;
        unsigned int numChannels;
        static constexpr size_t BUFFER_SIZE = 1048576;
};

#endif /* end of include guard: SAMPLEDATA_H_FIPJCQHI */
