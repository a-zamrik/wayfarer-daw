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

    for (int c = 0; c < channels; c++)
    {
        for (int i = 0; i < frame_size; i++)
        {
            frame(c,i) = this->get_next_sample();
        }
    }
}

float
Sine::get_next_sample()
{

    float next_sample = static_cast<float>(
                sin(( 2.0 * PI * hz * this->t ) / ( GConfig::get_instance().get_sample_rate() ))
            );

    this->t++;
    this->t = this->t % GConfig::get_instance().get_sample_rate(); // prevent overflow

    return next_sample;
}

void
CachedGenerator::filter(Frame & frame) {
    auto frame_size = GConfig::get_instance().get_frames_per_buffer();
    auto channels   = GConfig::get_instance().get_num_channels();

    for (int c = 0; c < channels; c++)
    {
        for (int i = 0; i < frame_size; i++)
        {
            if (wave_index == this->wave_table.size()) wave_index = 0;
            frame(c,i) = this->wave_table[wave_index];
            wave_index++;
        }
    }
}

CachedSine::CachedSine(int hz)
{
    auto sample_rate = GConfig::get_instance().get_sample_rate();

    // Generate the wave table. We only need one cycle at a resultion of the sample rate
    for (unsigned i = 0; i < sample_rate / hz; i++)
    {
        this->wave_table.push_back(
            static_cast<float>(
                sin(( 2.0 * PI * hz * i ) / ( sample_rate ))
            )
        );

        std::cout << static_cast<float>(
                sin(( 2.0 * PI * hz * i ) / ( sample_rate ))
            ) << std::endl;
    }

    std::cout << this->wave_table.size() << std::endl;

}