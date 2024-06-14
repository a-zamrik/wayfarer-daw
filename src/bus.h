#ifndef BUS_H
#define BUS_H

#include "oscillators.h"
#include "instrument.h"
#include "../include/portaudio.h"
#include "audio_track.h"
#include "effects/filters.h"
#include "./util/thread_safe_q.h"
#include <list>


class MasterBus : public WayfarerGuiComp
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


#ifdef USE_IMGUI
    TSQueue<std::vector<std::weak_ptr<WayfarerGuiComp>>> gui_effects_update_q;
    std::vector<std::weak_ptr<WayfarerGuiComp>>   gui_effects;
#endif

public:
    // Place holder variables
    Frame      frame;


    
    std::shared_ptr<SineSynth>  synth; 
    std::shared_ptr<AudioTrack> audio_track;


    // TODO: Change to use our linked list class
    // LinkedList<std::shared_ptr<AutoFilter>> effects;
    std::list<std::shared_ptr<AutoFilter>> effects;

    
    MasterBus() : frame(), gain(0.01f) { 
        synth = std::shared_ptr<SineSynth> (new SineSynth()); 
        effects.push_back( std::shared_ptr<AutoFilter> (new AutoFilter(0.707f, 500.f)) ); 
        effects.push_back( std::shared_ptr<AutoFilter> (new AutoFilter(0.707f, 1000.f)) );

#ifdef USE_IMGUI
        

        std::vector<std::weak_ptr<WayfarerGuiComp>> _gui_effects;
        _gui_effects.push_back(std::weak_ptr<WayfarerGuiComp>(this->synth));

        for (auto it = this->effects.begin(); it != this->effects.end(); it++)
        {
            _gui_effects.push_back(std::weak_ptr<WayfarerGuiComp>(*it));
        }

        this->gui_effects_update_q.push(_gui_effects);
#endif
    }

#ifdef USE_IMGUI
    virtual void draw_gui();
#endif

    // ~MasterBus()  { 
    //     delete synth;
    // }

    MasterBus& set_gain(float _gain);

    void init_stream();

    void start_stream();

    void stop_stream();

};

#endif