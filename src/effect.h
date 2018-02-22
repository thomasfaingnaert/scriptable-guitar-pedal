//
// Created by Robbe on 22/02/2018.
//

#ifndef SCRIPTABLE_GUITAR_PEDAL_EFFECT_H
#define SCRIPTABLE_GUITAR_PEDAL_EFFECT_H

/**
 * Abstract class for a guitar effect.
 */
template<typename T>
class Effect
{
public:
    /**
     *  Function to add an input sample to the buffer
     * @param sample contains the value of the sample to be added
     */
    virtual void addInputSample(T sample) = 0;

    /**
     * Function to get a processed sample from the output queue
     * @return a processed sample
     */
    virtual T getOutputSample() const = 0;
};


#endif //SCRIPTABLE_GUITAR_PEDAL_EFFECT_H
