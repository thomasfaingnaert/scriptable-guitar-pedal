#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

#include "NE10.h"
#include "filtereffect.h"

int main(int argc, char *argv[])
{
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    FilterEffect fe;
    fe.setImpulseResponse({1, 0, 0, 0});
    std::vector<Sample> block(fe.getBlockSize());
    std::iota(block.begin(), block.end(), 0);
    fe.addInputBlock(block);

    for (Sample s : fe.getOutputBlock())
    {
        std::cout << s << std::endl;
    }

    return EXIT_SUCCESS;
}
