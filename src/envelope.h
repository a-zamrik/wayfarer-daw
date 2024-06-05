#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_

#include <vector>
#include <memory>

class Envelope
{

    protected:
    std::vector<float> adsr_env;

    float attack_time;
    float decay_time;
    float release_time;

    float sustain_amp;
    int release_idx;

    int step = 0;

    public:
    Envelope(float atk_t, float atk_a, float dec_t, float rel_t, float sus_a);


    void reset() {this->step = 0;}

    float step_ads();
    float step_r();

};

#endif