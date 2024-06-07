#ifndef _AUDIO_TRACK_H_
#define _AUDIO_TRACK_H_

#include <vector>
#include "oscillators.h"

class AudioTrack
{

private:
    std::vector<std::vector<float>> samples;
    unsigned int play_index;

public:


    // TODO: resample to match global sample rate
    
    AudioTrack(std::vector<std::vector<float>> _samples);

    // You can't assign values to this like in a Frame
    inline float operator()(int c, int s) {return this->samples[c][s];} 
    void fill_frame(Frame & frame);

    size_t n_samples() {return this->samples[0].size();}


};

#endif