#ifndef CIRCULARBUFFER_H_G2CUFH3V
#define CIRCULARBUFFER_H_G2CUFH3V

#include <vector>

template<class T>
class CircularBuffer
{
    public:
        CircularBuffer(unsigned int size)
            : data(size), readIndex(0), writeIndex(0)
        {
        }

        T* getNextReadPointer(unsigned int size)
        {
            if (readIndex + size > data.size())
                throw std::invalid_argument("Unaligned access to CircularBuffer");

            T* ret = data.data() + readIndex;
            readIndex = (readIndex + size) % data.size();
            return ret;
        }

        T* getNextWritePointer(unsigned int size)
        {
            if (writeIndex + size > data.size())
                throw std::invalid_argument("Unaligned access to CircularBuffer");

            T* ret = data.data() + writeIndex;
            writeIndex = (writeIndex + size) % data.size();
            return ret;
        }

    private:
        std::vector<T> data;
        unsigned int readIndex;
        unsigned int writeIndex;
};

#endif /* end of include guard: CIRCULARBUFFER_H_G2CUFH3V */
