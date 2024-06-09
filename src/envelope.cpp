#include "envelope.h"
#include "global_config.h"
#include "error.h"
#include <cmath>


float 
lerp(float a, float b, float t) {
  return (1 - t) * a + t * b;
}


RollingAverage::RollingAverage(float time_window_s)
{
    uint32_t buff_size = (uint32_t) (GConfig::get_instance().get_sample_rate() * time_window_s);

    if (buff_size == 0)
    {
        critical_error("RollingAvage circular buffer size is zero. time_window too small\n");
    }
    
    for (uint32_t i = 0; i < buff_size; i++)
    {
        this->circ_buff.push_back(0);
    }

    this->c_buff_index = 0;
    this->avg = 0;
}

float 
RollingAverage::add_sample(float s)
{
    float avg_without_last = (this->avg * ((float) this->circ_buff.size())) - this->circ_buff[this->c_buff_index];

    this->avg =  (avg_without_last + s) / this->circ_buff.size();

    this->circ_buff[this->c_buff_index] = s;

    this->c_buff_index++;
    if (this->c_buff_index >= this->circ_buff.size())
    {
        this->c_buff_index = 0;
    }

    return this->avg;
}

Envelope::Envelope(float atk_t, float atk_a, float dec_t, float rel_t, float sus_a)
{

    this->attack_time = atk_t;
    this->decay_time = dec_t;
    this->release_time = rel_t;


    this->attack_n_samples   = GConfig::get_instance().get_sample_rate() * atk_t;
    this->decay_n_samples    = GConfig::get_instance().get_sample_rate() * dec_t;
    this->release_n_samples  = GConfig::get_instance().get_sample_rate() * rel_t;

    // Place step at end of envelope. wait for reset to be called
    this->step = static_cast<int>(this->attack_n_samples + this->decay_n_samples + this->release_n_samples);

    this->sustain_amp = sus_a;
    this->attack_amp  = atk_a;
}

float 
Envelope::__step_a()
{

    float out = (static_cast<float>(this->step) /  attack_n_samples) * this->attack_amp;
    this->step++;
    return out;

}

float 
Envelope::__step_d()
{

    float ratio = ( (static_cast<float>(this->step) - this->attack_n_samples) /  decay_n_samples);
    float out = lerp(this->attack_amp, this->sustain_amp, ratio);
    this->step++;
    return out;

}

float
Envelope::step_ads()
{
    if (this->step <= this->attack_n_samples)
    { // In attack portion
        return this->__step_a();
    } 
    else if (this->step <= (this->attack_n_samples + this->decay_n_samples)) 
    { // Decay portion
        return this->__step_d();
    } 
    else 
    {  // Sustain portion. If caller needs release, they will call step_r 
        return this->sustain_amp;
    }
}

float
Envelope::step_r()
{
    if (this->step <= (this->attack_n_samples + this->decay_n_samples))
    {  // Release was called before we reached sustain part. jump to release part
        this->step = static_cast<int>(this->attack_n_samples + this->decay_n_samples);
    } 

    if (this->is_done())
    {   // Envelope finished, Reset must be called to restart
        return 0;
    } 
    else
    {   // Calculate release value
        float ratio = ((static_cast<float>(this->step) - this->attack_n_samples - this->decay_n_samples) /  this->release_n_samples);
        float out = lerp(this->sustain_amp, 0.0f, ratio);
        this->step++;
        return out;
    }
}

inline bool
Envelope::is_done()
{
    return this->step >= (this->attack_n_samples + this->decay_n_samples + this->release_n_samples);
}