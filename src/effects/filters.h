#ifndef _FILTERS_H_
#define _FILTERS_H_

#include "../error.h"
#include <stdio.h>
#include "../gui/gui.h"

/*  References:
    https://arachnoid.com/BiQuadDesigner/index.html
    https://shepazu.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
    https://en.wikipedia.org/wiki/Digital_biquad_filter

*/


class Frame;


class BiQuadFilter : public WayfarerGuiComp
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

    // Param checks
    float max_freq;
    float min_freq = 20.f;

    // Abstract func, children need to implement
    virtual void __recalculate_coefficients() { critical_error("Not Implemented"); }

    // calculate frequency domain value at the given freq
    float __freq_domain_at(float freq);

    // Populate a preq table given the max_freq
    void __populate_freq_domain_table(float *array, uint32_t count);


    float __freq_response_table[40];
#ifdef USE_IMGUI
    virtual void draw_gui() { critical_error ("Not Implemented");}
#endif

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
        this->__populate_freq_domain_table(this->__freq_response_table, (uint32_t) std::size(this->__freq_response_table));

    }

};

class AutoFilter : public BiQuadFilter
{
private:
    enum FilterType {
        HighPass,
        LowPass,
        BandPass,
        Notch
    };

    FilterType filter_type = FilterType::LowPass;

    void __normalize_coefficients();

    void __make_lowpass();
    void __make_highpass();
    void __make_bandpass();
    void __make_notch();

    uint32_t instance_id;
    static uint32_t instance_count;

protected:

    

    virtual void __recalculate_coefficients();
#ifdef USE_IMGUI
    virtual void draw_gui();
#endif
    
public:
    bool deleted = false;
    int  requested_position_in_chain = -1;
    AutoFilter(float _q, float _center_freq)
    {
        instance_id = AutoFilter::instance_count++;
        this->set_params(_q, _center_freq);
    }

};


#endif