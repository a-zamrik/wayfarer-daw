#ifndef BUS_H
#define BUS_H

#include "oscillators.h"
#include "instrument.h"
#include "../include/portaudio.h"
#include "audio_track.h"
#include "effects/filters.h"

class MasterBus
{
private:



    // TODO: Instrument / Generator / Audio Input

    // TODO: Audio Effects
    // std::vector<AudioEffect> effect_chain;

    float gain;

    PaStream *stream;
    static int paCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData );

public:
    // Place holder variables
    Frame      frame;


    // TODO: MAKE A SHARED POINTER
    std::shared_ptr<SineSynth>  synth; // Dangerous to expose
    std::shared_ptr<AudioTrack> audio_track;

    LowpassFilter lowpass_filter;

    
    MasterBus() : frame(), gain(0.01f) { 
        synth = std::shared_ptr<SineSynth> (new SineSynth()); 
        WayfarerGUI::get_instance().register_comp(this->synth);
    }

    // ~MasterBus()  { 
    //     delete synth;
    // }

    MasterBus& set_gain(float _gain);

    void init_stream();

    void start_stream();

    void stop_stream();

};

#endif