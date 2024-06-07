#include "audio_track.h"
#include "global_config.h"


AudioTrack::AudioTrack(std::vector<std::vector<float>> _samples)
{
    this->play_index = 86800;
    this->samples = _samples; // copy the created samples
}

void
AudioTrack::fill_frame(Frame & frame)
{
    auto channels = GConfig::get_instance().get_num_channels();
    auto frames = GConfig::get_instance().get_frames_per_buffer();

    for (int s = 0; s < frames; s++)
    {
        for (int c = 0; c < channels; c++)
        {
            frame(c, s) += this->samples[c][this->play_index + s];

            //printf("%f\n", this->samples[c][this->play_index + s]);
        }
    }

    this->play_index += frames;
    //printf("%u\n", this->play_index);
}