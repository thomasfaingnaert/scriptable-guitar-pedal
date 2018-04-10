#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <vector>

#include "NE10.h"
#include "luaeffect.h"

int main(int argc, char *argv[])
{
    // NE10 Initialisation
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    LuaEffect effect("examples/test.lua");

    auto data = std::make_shared<std::vector<float>>(10);
    std::iota(data->begin(), data->end(), 0);

    auto result = effect.process({data});

    std::cout << "Got result:\n";
    std::copy(result->begin(), result->end(), std::ostream_iterator<float>(std::cout, " "));

    std::cout << "\n";

    return EXIT_SUCCESS;
}

