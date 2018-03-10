#include <string>

#include "sink.h"

#ifndef FILESINK_H_6ZZMAMTF
#define FILESINK_H_6ZZMAMTF

class FileSink : public Sink<float>
{
    public:
        FileSink(const std::string& filename) : filename(filename) { }
        virtual void push(const std::shared_ptr<std::vector<float>>& t, unsigned int channel);
        void write();

    private:
        std::string filename;
        std::vector<float> samples;
};

#endif /* end of include guard: FILESINK_H_6ZZMAMTF */
