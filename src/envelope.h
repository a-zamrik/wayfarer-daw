#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_

#include <vector>
#include <memory>


class RollingAverage
{
    std::vector<float> circ_buff;
    int c_buff_index; // Will point to the last added sample, just before adding
    float avg;

public:

    RollingAverage(float time_window_s);

    // Returns the average after adding the new sample s
    float add_sample(float s);
    inline float get_avg() const {return this->avg;}
};



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

    // Envlope has finished; this->step is at end of envelope, need to call reset()
    bool is_done();

    void set_atk_amp(float amp) {this->attack_amp  = amp;}
    void set_sus_amp(float amp) {this->sustain_amp = amp;}


    // void to_float_array(float * array, uint32_t count);

};

#endif