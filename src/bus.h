#ifndef BUS_H
#define BUS_H

#include "oscillators.h"
#include "instrument.h"
#include "../include/portaudio.h"
#include "audio_track.h"

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
    SineSynth*  synth; // Dangerous to expose
    std::shared_ptr<AudioTrack> audio_track;

    
    MasterBus() : frame(), gain(0.01f) { 
        synth = new SineSynth(); 
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