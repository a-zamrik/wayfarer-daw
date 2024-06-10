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


    
    std::shared_ptr<SineSynth>  synth; 
    std::shared_ptr<AudioTrack> audio_track;

    std::vector<std::shared_ptr<AutoFilter>> effects;

    
    MasterBus() : frame(), gain(0.01f) { 
        synth = std::shared_ptr<SineSynth> (new SineSynth()); 
        effects.push_back( std::shared_ptr<AutoFilter> (new AutoFilter(0.707f, 500.f)) ); 
        effects.push_back( std::shared_ptr<AutoFilter> (new AutoFilter(0.707f, 1000.f)) );
        WayfarerGUI::get_instance().register_comp(this->synth);
        WayfarerGUI::get_instance().register_comp(this->effects[0]);
        WayfarerGUI::get_instance().register_comp(this->effects[1]);
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