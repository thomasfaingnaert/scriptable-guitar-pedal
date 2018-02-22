//
// Created by Robbe on 22/02/2018.
//

#ifndef SCRIPTABLE_GUITAR_PEDAL_BLOCK_EFFECT_H
#define SCRIPTABLE_GUITAR_PEDAL_BLOCK_EFFECT_H

#include <effect.h>

/**
 * Class to implement a block effect.
 * What is meant by this, is that samples are first added to a buffer and then the buffer is processed.
 */
template<typename T, unsigned BLOCK_SIZE>
class BlockEffect : public Effect<T>
{
private:
    /**
     * Array containing a buffer of samples
     */
    std::array <T, BLOCK_SIZE> block_buffer;

    /**
     * An iterator to help with the array processing
     */
    std::array<T, BLOCK_SIZE>::iterator it;

    /**
     * function to process the samples block per block.
     * This function will be called by the 'addInputSample' routine after the block_buffer is full.
     *
     * NOTE: do not forget to set iterator to begin.
     */
    virtual void processBlock(const std::array <T, BLOCK_SIZE>& block_buffer) = 0;
};

#endif //SCRIPTABLE_GUITAR_PEDAL_BLOCK_EFFECT_H
