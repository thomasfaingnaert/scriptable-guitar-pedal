#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
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
    std::vector<Complex> freq = fe.getFrequencyResponse();

    for (Complex c : freq)
    {
        std::cout << c.r << " + i " << c.i << std::endl;
    }

    return EXIT_SUCCESS;
}
