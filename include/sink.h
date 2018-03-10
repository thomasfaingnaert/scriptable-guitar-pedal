#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>

#ifndef SINK_H_FUKZZ1SW
#define SINK_H_FUKZZ1SW

template<typename T>
class Sink
{
    public:
        std::size_t getBlockSize(int channel) const { return channel+1; }

        void push(const T* t, std::size_t n, int channel) const
        {
            std::cout << "channel " << channel << ": ";
            std::copy(t, t + n, std::ostream_iterator<T>(std::cout, " "));
            std::cout << "\n";
        }

    private:
};

#endif /* end of include guard: SINK_H_FUKZZ1SW */
