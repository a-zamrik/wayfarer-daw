#include "generators.h"
#include "global_config.h"
#include <math.h>
#include <cassert>
#include <iostream>

#define PI (3.14159265359)

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

float
Sine::get_next_sample()
{

    // TODO: When the sample is zero, we should do the increment

    float next_sample = static_cast<float>(
                sin(( 2.0 * PI * this->hz * this->t ) / ( GConfig::get_instance().get_sample_rate() ))
            );

    this->t++;
    this->t = this->t % GConfig::get_instance().get_sample_rate(); // prevent overflow

    // Want to chang hz when the sine wave equates to zero
    if (this->change_hz &&  std::abs(next_sample) <= 1e-4)
    {
        printf("%f\n", next_sample);
        this->hz += this->change_hz;
        this->change_hz = 0;
    }

    return next_sample;
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