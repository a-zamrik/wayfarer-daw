#include "oscillators.h"
#include "global_config.h"
#include <math.h>
#include <cassert>
#include <iostream>

#define PI  (3.14159265359)
#define TWO_PI (2.0 * PI)

Frame::Frame()
{
    auto channels = GConfig::get_instance().get_num_channels();
    auto frames = GConfig::get_instance().get_frames_per_buffer();

    for (int c = 0; c < channels; c++)
    {
        samples.push_back(std::vector<float>());
        for (int s = 0; s < frames; s++)
        {
            samples[c].push_back(0.0);
        }
    }

    assert (channels == samples.size() && frames == samples[0].size());

    return;
}

void
Generator::filter(Frame & frame)
{
    auto frame_size = GConfig::get_instance().get_frames_per_buffer();
    auto channels   = GConfig::get_instance().get_num_channels();

    for (int i = 0; i < frame_size; i++)
    {
        float sample = this->get_next_sample();
        for (int c = 0; c < channels; c++)
        {
            frame(c,i) = sample;
        }
    }
}

void
Sine::set_hz(float _new_hz)
{
    if (_new_hz < 20.0f || _new_hz > 20000.0f) { return; }

    int last_t = this->t - 1;

    // Need phase of new sine wave to match the phase of old wave to prevent clicking
    float current_phase = static_cast<float>(TWO_PI * this->hz * last_t + this->phase_shift);
    this->phase_shift = static_cast<float>(current_phase - (TWO_PI * _new_hz * last_t));
    this->hz = _new_hz;
}

float
Sine::get_next_sample()
{

    //printf("%u\n", this->adsr_idx);
    float env_val;
    if (this->on) 
    {
        env_val = this->adsr_env.step_ads();
        //printf("%f\n", env_val);
        //env_val = 1.0f;
    }
    else
    {
        env_val = this->adsr_env.step_r();
        //env_val = 0.0f;
    }
    //printf("%f\n", env_val);

    float next_sample = static_cast<float>(
                sin(( ( TWO_PI * this->hz * this->t) + this->phase_shift ) / ( GConfig::get_instance().get_sample_rate() ))
            );

    // TODO: NEED OT PREVENT OVERFLOW?
    this->t++;

    env_val = this->adsr_env_rolling_avg.add_sample(env_val);

    return next_sample * env_val;
}

bool
Sine::is_outputing()
{
    return this->on || (!this->on && !this->adsr_env.is_done());
}

void
WaveTable::filter(Frame & frame) {
    auto frame_size = GConfig::get_instance().get_frames_per_buffer();
    auto channels   = GConfig::get_instance().get_num_channels();

    for (int i = 0; i < frame_size; i++)
    {
        float sample = this->get_next_sample();
        for (int c = 0; c < channels; c++)
        {
            frame(c,i) = sample;
        }
    }
}


inline float
WaveTable::get_next_sample() {

    int wi;
    float frac;

    if (wave_index >= this->wave_table.size())
    {
        wave_index = 0.f;
    }

    wi = (int) wave_index;
    frac = wave_index - wi;


    float sample = (this->wave_table[wi] * (1.0f - frac)) + (this->wave_table[(wi + 1) % this->wave_table.size()] * (frac));
    wave_index += wave_step;
        
    return sample;
}


WTSine::WTSine(float _min_hz, float _hz)
{
    this->min_hz = _min_hz;
    this->hz     = _hz;
    this->wave_step = 1;

    auto sample_rate = GConfig::get_instance().get_sample_rate();

    // Generate the wave table. We only need one cycle at a resultion of the sample rate
    for (unsigned i = 0; i < (sample_rate / _min_hz); i++)
    {
        this->wave_table.push_back(
            static_cast<float>(
                sin(( 2.0 * PI * _min_hz * i ) / ( sample_rate ))
            )
        );

        std::cout << static_cast<float>(
                sin(( 2.0 * PI * hz * i ) / ( sample_rate ))
            ) << std::endl;
    }

    std::cout << this->wave_table.size() << std::endl;

}