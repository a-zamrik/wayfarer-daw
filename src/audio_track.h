#ifndef _AUDIO_TRACK_H_
#define _AUDIO_TRACK_H_

#include <vector>
#include "oscillators.h"

class AudioTrack
{

private:
    std::vector<std::vector<float>> samples;
    uint32_t play_index;
    uint32_t sample_rate;

public:


    // TODO: resample to match global sample rate
    
    AudioTrack(std::vector<std::vector<float>> _samples, uint32_t _sample_rate);

    // You can't assign values to this like in a Frame
    inline float operator()(int c, int s) {return this->samples[c][s];} 
    void fill_frame(Frame & frame);

    size_t n_samples() {return this->samples[0].size();}

    /* Resample audio track to new_sample_rate.
       Used to have song play at same pitch and tempo if tracks sample rate does not match wayfarer's sample rate

        NOTE: Sampling below 40 kHz will cause a low-pass effect. Nyquist frequency is 20 kHz for human hearing, 
              Therefore, we need a sample rate of 40 kHz at least to hear all frequencies
    
        NOTE: Retargetting to a significantly lower sample rate will cause artifacts
    */
    void target_sample_rate(uint32_t new_sample_rate);




};

#endif