#include "filters.h"
#include "../oscillators.h"
#include "../global_config.h"
#include "../util/wmath.h"
#include <cmath>
#include <iostream>

void
BiQuadFilter::filter_frame(Frame & frame)
{
    auto frame_size = GConfig::get_instance().get_frames_per_buffer();
    auto channels   = GConfig::get_instance().get_num_channels();

    for (int i = 0; i < frame_size; i++)
    {
        for (int c = 0; c < channels; c++)
        {
            frame(c,i) = this->filter(frame(c,i));
        }
    }
}

float
BiQuadFilter::filter(float sample)
{
    float y = (b0 * sample) + (b1 * x1) + (b2 * x2) - (a1 * y1) - (a2 * y2);
    x2 = x1;
    x1 = sample;
    y2 = y1;
    y1 = y;
    return y;
}

void
LowpassFilter::__recalculate_coefficients()
{
    auto sample_rate = GConfig::get_instance().get_sample_rate();
    float omega = TWO_PI_F * this->center_freq / sample_rate;
    float alpha = sin(omega) / (2.0f * this->q);

    
    
    this->b0 = (1.0f - cos(omega)) / 2.0f;
    this->b1 = (1.0f - cos(omega));
    this->b2 = (1.0f - cos(omega)) / 2.0f;

    this->a0 = 1.0f + alpha;
    this->a1 = -2.0f * cos(omega);
    this->a2 = 1.0f - alpha;

    // For optimization, we can normalize a0 to be equal to 1
    // afterwhich, a division by 1, has no effect on result.
    this->b0 /= this->a0;
    this->b1 /= this->a0;
    this->b2 /= this->a0;
    this->a1 /= this->a0;
    this->a2 /= this->a0;


    std::cout << "b0:" << b0 << std::endl
              << "b1:" << b1 << std::endl
              << "b2:" << b2 << std::endl
              << "a1:" << a1 << std::endl
              << "a2:" << a2 << std::endl;

}