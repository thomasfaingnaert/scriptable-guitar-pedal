#include <cstddef>
#include <vector>

#ifndef BLOCKBUFFER_H_97TSLADB
#define BLOCKBUFFER_H_97TSLADB

template<typename T>
class BlockBuffer
{
    public:
        BlockBuffer(std::size_t numBlocks, std::size_t blockSize)
            : numBlocks(numBlocks), blockSize(blockSize), index(0), buffer(2 * numBlocks * blockSize)
        {
        }

        void push_back(const std::vector<T>& v)
        {
            if (v.size() != blockSize)
                throw std::invalid_argument("New element should have size blockSize");

            // write new element twice to allow contiguous storage in memory
            std::copy(v.begin(), v.end(), buffer.begin() + index);
            std::copy(v.begin(), v.end(), buffer.begin() + index + size());

            // go to next block
            if (index == size() - blockSize)
                index = 0;
            else
                index += blockSize;
        }

        std::size_t size() const
        {
            return numBlocks * blockSize;
        }

        typedef const T* const_iterator;
        const_iterator begin() const { return buffer.data() + index; }
        const_iterator end() const { return begin() + size(); }
        const T* data() const { return begin(); }

    private:
        std::size_t numBlocks; // number of blocks in buffer
        std::size_t blockSize; // number of elements in one block
        std::size_t index; // index of current "first element" in buffer
        std::vector<T> buffer; // buffer that contains each block twice to allow contiguous storage
};

#endif /* end of include guard: BLOCKBUFFER_H_97TSLADB */
