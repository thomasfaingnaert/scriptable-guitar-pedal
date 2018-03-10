#include <cstddef>
#include <vector>

#ifndef SAMPLEBUFFER_H_M3USJL6E
#define SAMPLEBUFFER_H_M3USJL6E

// circular buffer with contiguous storage
template<typename T>
class SampleBuffer
{
    public:
        SampleBuffer(size_t size)
            : size(size), index(0)
        {
            // buffer is twice as big as number of elements
            buf.resize(2 * size);
        }

        void push_back(const T& t)
        {
            // write new element in both copies of the block
            buf[index] = buf[index + size] = t;

            // go to next element
            if (index == size - 1)
                index = 0;
            else
                ++index;
        }

        T* data() { return buf.data() + index; }
        const T* data() const { return buf.data() + index; }

    private:
        const std::size_t size; // maximum number of elements
        std::size_t index; // index of first item in block
        std::vector<T> buf; // buffer that contains the block twice to allow contiguous storage
};

#endif /* end of include guard: SAMPLEBUFFER_H_M3USJL6E */
