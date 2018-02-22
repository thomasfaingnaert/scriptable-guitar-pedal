#ifndef SCRIPTABLE_GUITAR_PEDAL_SAMPLE_EFFECT_H
#define SCRIPTABLE_GUITAR_PEDAL_SAMPLE_EFFECT_H

#include <effect.h>

/**
 * Class for implementing a sample based effect.
 * What is meant by this, is that samples should be processed one by one.
 */
template <typename T> class SampleEffect: public Effect<T> {
private:
    /**
     * Variable containing the current sample to be processed
     */
    T sample;

    /**
     * Function to process the sample that was added last.
     */
    virtual void processSample() = 0;
};


#endif
