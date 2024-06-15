#include "audio_track.h"
#include "global_config.h"
#include "./util/wmath.h"


AudioTrack::AudioTrack(std::vector<std::vector<float>> _samples, uint32_t _sample_rate)
{
    this->play_index = 0;
    this->samples = _samples; // copy the created samples
    this->sample_rate = _sample_rate;
}

void
AudioTrack::fill_frame(Frame & frame)
{
    auto channels = GConfig::get_instance().get_num_channels();
    size_t frames = GConfig::get_instance().get_frames_per_buffer();


    // Make sure we don't index OOR
    size_t samples_left = this->samples[0].size() - this->play_index;
    if (samples_left <= frames)
    {
        frames = samples_left;
    }

    for (int s = 0; s < frames; s++)
    {
        for (int c = 0; c < channels; c++)
        {
            frame(c, s) += this->samples[c][this->play_index + s];

            //printf("%f\n", this->samples[c][this->play_index + s]);
        }
    }

    this->play_index += (uint32_t) frames;
    //printf("%u\n", this->play_index);
}

void 
AudioTrack::target_sample_rate(uint32_t new_sample_rate)
{
    // TODO: make this pre allocate size
    std::vector<std::vector<float>> new_samples;

    // create same number of channels
    for (int c = 0; c < this->samples.size(); c++)
    {
        new_samples.push_back(std::vector<float>());
    }

    float step = ((float) this->sample_rate) / ((float) new_sample_rate);

    float interpolated_sample = 0;
    float lerp_ratio = 0;

    
    for (float s = 0; s < this->samples[0].size() - 1; s += step)
    {
        for (int c = 0; c < this->samples.size(); c++)
        {
            int sample_index = (int) s;
            lerp_ratio = s - sample_index;
            float interpolated_sample = lerp(this->samples[c][sample_index], this->samples[c][sample_index + 1], lerp_ratio);

            new_samples[c].push_back(interpolated_sample);
        }
    }

    this->samples = new_samples;
    this->sample_rate = new_sample_rate;
}