#include <algorithm>
#include <iostream>
#include <iterator>
#include <iomanip>

#include "frequencydelayline.h"

int main(int argc, char *argv[])
{
    // NE10 Initialisation
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    static constexpr unsigned int BLOCK_SIZE = 16;


    std::array<float, BLOCK_SIZE+1> impulse1{0,1};
    std::array<float, BLOCK_SIZE+1> impulse2{};
    FrequencyDelayLine<BLOCK_SIZE> fdl({impulse1,impulse2});

    std::array<float, BLOCK_SIZE> input{1,2,3,4};
    auto res = fdl.process(input);

    std::cout << std::setprecision(3);
    std::copy(res.begin(), res.end(), std::ostream_iterator<float>(std::cout, " "));
    std::cout << "\n";

    return EXIT_SUCCESS;
}

