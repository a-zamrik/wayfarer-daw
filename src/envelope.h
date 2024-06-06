#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_

#include <vector>
#include <memory>

class Envelope
{

    protected:

    float attack_n_samples;
    float decay_n_samples;
    float release_n_samples;

    float attack_time;
    float decay_time;
    float release_time;

    float attack_amp;
    float sustain_amp;
    int release_idx;

    int step = 0;

    float __step_a();
    float __step_d();


    public:
    Envelope(float atk_t, float atk_a, float dec_t, float rel_t, float sus_a);


    void reset() {this->step = 0;}

    float step_ads();
    float step_r();

};

#endif