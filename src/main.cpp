#include <cstdlib>
#include <iostream>

#include "NE10.h"

int main(int argc, char *argv[])
{
    if (ne10_init() != NE10_OK)
    {
        std::cout << "Could not initialise Ne10.\n";
        return EXIT_FAILURE;
    }

    std::cout << "Successfully initialised Ne10.\n";
    return EXIT_SUCCESS;
}
