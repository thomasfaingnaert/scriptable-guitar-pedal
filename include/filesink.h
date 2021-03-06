#ifndef FILESINK_H_6ZZMAMTF
#define FILESINK_H_6ZZMAMTF

#include <array>
#include <deque>
#include <string>

#include "constants.h"
#include "sink.h"

class FileSink : public Sink<float>
{
    public:
        FileSink(const std::string& filename, unsigned int samplerate);
        virtual void push(const std::array<float, Constants::BLOCK_SIZE>& data);
        void write(bool normalize = false);

    private:
        std::string filename;
        unsigned int samplerate;
        std::deque<float> samples;
};

#endif /* end of include guard: FILESINK_H_6ZZMAMTF */
