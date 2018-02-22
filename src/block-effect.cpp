#include <iostream>

#include "block-effect.h"

virtual void BlockEffect<T, BLOCK_SIZE>::addInputSample(T sample)
{
    if (it != block_buffer.end)
    {
        *it++ = sample; // add sample to array
    }
    else
    {
        // give control to block processing
        processBlock(block_buffer);
        // add sample to the now 'empty' buffer
        addInputSample(sample);
    }
};

virtual T BlockEffect<T, BLOCK_SIZE>::getOutputSample()
{
    if (it != block_buffer.begin)
    {
        return *it--;
    }
};
