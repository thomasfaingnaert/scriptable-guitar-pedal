#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

#include "NE10.h"
#include "filtereffect.h"
#include "sampledata.h"

int main(int argc, char *argv[])
{
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    SampleData input("smoke-on-the-water.wav");
    SampleData filter("SquareVictoriaDome.wav");

    std::vector<Sample> inputSamples = input.getSamples()[0];
    std::vector<Sample> filterSamples = filter.getSamples()[0];

    FilterEffect fe;
    fe.setImpulseResponse(filterSamples);

    std::vector<Sample> outputSamples;

    unsigned int pos = 0;

    while (pos + fe.getBlockSize() < inputSamples.size())
    {
        std::vector<Sample> block(inputSamples.begin() + pos, inputSamples.begin() + pos + fe.getBlockSize());
        fe.addInputBlock(block);
        std::vector<Sample> output = fe.getOutputBlock();
        outputSamples.insert(outputSamples.end(), output.begin(), output.end());
        pos += fe.getBlockSize();
    }

    Sample max = outputSamples[0];
    for (Sample sample : outputSamples)
    {
        max = (sample > max) ? sample : max;
    }

    for (Sample& sample : outputSamples)
    {
        sample /= max;
    }

    SampleData out({ outputSamples }, input.getSampleRate());
    out.save("output.wav");

    return EXIT_SUCCESS;
}
