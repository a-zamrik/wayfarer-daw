#ifndef _FILTERS_H_
#define _FILTERS_H_

#include "../error.h"
#include <stdio.h>

/*  References:
    https://arachnoid.com/BiQuadDesigner/index.html
    https://shepazu.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html

*/


class Frame;


class BiQuadFilter
{

protected:
    float x1, x2; // last two input samples. t = now, then x1 = input[t-1] and so on
    float y1, y2; // last two output samples

    // BiQuadratic coefficients
    float b0, b1, b2;
    float a0, a1, a2;

    // Params
    float q;
    float center_freq;

    // Abstract func, children need to implement
    virtual void __recalculate_coefficients() { critical_error("Not Implemented"); }

public:

    BiQuadFilter() : x1(0) , x2(0), 
                     y1(0), y2(0), 
                     b0(0), b1(0), b2(0), 
                     a0(0), a1(0), a2(0), 
                     q(0),  center_freq(0) {}

    void filter_frame(Frame & f);

    float filter(float sample);

    void set_params(float _q, float _center_freq) {
        this->q = _q; 
        this->center_freq = _center_freq; 
        this->__recalculate_coefficients();
    }

};

class LowpassFilter : public BiQuadFilter
{

protected:
    virtual void __recalculate_coefficients();
    
public:
    LowpassFilter()
    {
        a1 = -1.96297470f;
        a2 = 0.96364759f;
        b0 = 1.68223247e-4f;
        b1 = 3.36446494e-4f;
        b2 = 1.68223247e-4f;
    }

};

#endif