#include "envelope.h"
#include "global_config.h"

Envelope::Envelope(float atk_t, float atk_a, float dec_t, float rel_t, float sus_a)
{
    int attack_n_samples   = static_cast<int>( GConfig::get_instance().get_sample_rate() * atk_t);
    int decay_n_samples    = static_cast<int>( GConfig::get_instance().get_sample_rate() * dec_t);
    int release_n_samples  = static_cast<int>( GConfig::get_instance().get_sample_rate() * rel_t);

    // Create attack rise
    for (int i = 0; i < attack_n_samples; i++)
    {
        this->adsr_env.push_back(((float) i / (float) attack_n_samples) * atk_a);
    }

    // Create decay to sustain
    for (int i = 0; i < decay_n_samples; i++)
    {
        this->adsr_env.push_back((1.0f - ((float) i / (float) decay_n_samples)) * sustain_amp);
    }

    // This is the index we will sit at if the note is being held
    this->release_idx = static_cast<int>(this->adsr_env.size() - 1);

    // Create sustain to release
    for (int i = 0; i < release_n_samples; i++)
    {
        this->adsr_env.push_back((1.0f - ((float) i / (float) release_n_samples)) * (sus_a));
    }

    this->step = this->adsr_env.size();
    this->sustain_amp = sus_a;
}

float
Envelope::step_ads()
{
    if (this->step <= this->release_idx)
    {
        return this->adsr_env[this->step++];
    } else {
        
        return this->sustain_amp;
    }
}

float
Envelope::step_r()
{
    if (this->step <= this->release_idx)
    {
        this->step = this->release_idx;
    } 

    if (this->step >= this->adsr_env.size())
    {
        return 0;
    }

    return this->adsr_env[this->step++];
}