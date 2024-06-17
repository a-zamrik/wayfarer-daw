#ifndef BUS_H
#define BUS_H

#include "oscillators.h"
#include "instrument.h"
#include "../include/portaudio.h"
#include "audio_track.h"
#include "effects/filters.h"
#include "./util/thread_safe_q.h"
#include "./util/linked_list.h"
#include <list>
#include "sequence.h"


class Bus : public WayfarerGuiComp
{

private:
    std::shared_ptr<SineSynth>  synth; 
    std::shared_ptr<AudioTrack> audio_track;

protected:

    float gain;

#ifdef USE_IMGUI
    TSQueue<std::vector<std::weak_ptr<WayfarerGuiComp>>> gui_effects_update_q;
    std::vector<std::weak_ptr<WayfarerGuiComp>>   gui_effects;
#endif

public:

    // TODO: NEED TO STORE MIDI NOTE SEGMENTS SOME HOW AND READ FROM THEM.
    //       CREATE MIDI NOTE SEQUENCER CLASS OR SOMETHIGN

    
    std::shared_ptr<SineSynth> & get_instrument() {return this->synth;}
    std::shared_ptr<AudioTrack> & get_audio_track() {return this->audio_track;}

    void set_instrument(std::shared_ptr<SineSynth> & inst) {this->synth = inst;}
    void set_audio_track(std::shared_ptr<AudioTrack> & audio) {this->audio_track = audio;}



    // TODO: Change to use our linked list class
    // LinkedList<std::shared_ptr<AutoFilter>> effects;
    LinkedList<std::shared_ptr<AutoFilter>> effects;
    std::mutex effects_lock;


    std::shared_ptr<MidiSequence> midi_sequence;

    
    Bus() :  gain(0.01f) { 
        synth = std::shared_ptr<SineSynth> (new SineSynth()); 
        effects.push_back( std::shared_ptr<AutoFilter> (new AutoFilter(0.707f, 500.f)) ); 
        effects.push_back( std::shared_ptr<AutoFilter> (new AutoFilter(0.707f, 1000.f)) );
        effects.push_back( std::shared_ptr<AutoFilter> (new AutoFilter(0.707f, 500.f)) ); 
        update_chain_oder();

        midi_sequence = std::shared_ptr<MidiSequence>(new MidiSequence());

        midi_sequence->bound_instrument = std::weak_ptr<SineSynth>(synth);
        synth->bound_sequence = std::weak_ptr<MidiSequence>(midi_sequence);

        //midi_sequence->add_note(0, 3.0f, 1.f);

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

    virtual Bus& set_gain(float _gain);

    void render_frame(Frame & frame);

    
    virtual void add_effect_at(uint64_t effect_id, uint32_t index);
    virtual void move_effect_to(uint64_t src_idx, uint32_t dest_idx);
    
    virtual void update_chain_oder()
    {
        unsigned i = 0;
        for (auto it = this->effects.begin(); it != this->effects.end(); it++)
        {
            (*it)->chain_order = i;
            i++;
        }

    }

};


class MasterBus : public Bus
{
private:

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
    Frame      frame;

    LinkedList<std::shared_ptr<Bus>> busses;
    std::mutex busses_lock;

    // TODO: Change to use our linked list class
    // LinkedList<std::shared_ptr<AutoFilter>> effects;
    LinkedList<std::shared_ptr<AutoFilter>> effects;
    std::mutex effects_lock;

    void add_bus(std::shared_ptr<Bus> & bus) {
        this->busses_lock.lock();
        this->busses.push_back(bus);
        this->busses_lock.unlock();
    }

    void init_stream();
    void start_stream();
    void stop_stream();


    MasterBus() : frame(), gain(0.01f) { 
        effects.push_back( std::shared_ptr<AutoFilter> (new AutoFilter(0.707f, 500.f)) ); 
        effects.push_back( std::shared_ptr<AutoFilter> (new AutoFilter(0.707f, 1000.f)) );
        effects.push_back( std::shared_ptr<AutoFilter> (new AutoFilter(0.707f, 500.f)) ); 
        update_chain_oder();

#ifdef USE_IMGUI
        
        std::vector<std::weak_ptr<WayfarerGuiComp>> _gui_effects;

        for (auto it = this->effects.begin(); it != this->effects.end(); it++)
        {
            _gui_effects.push_back(std::weak_ptr<WayfarerGuiComp>(*it));
        }

        this->gui_effects_update_q.push(_gui_effects);
#endif
    }

#ifdef USE_IMGUI
    //virtual void draw_gui();
#endif


};

#endif


//   0    1    2    3 
// 0 [] 1 [] 2 [] 3 [] 4 

// 0 1 -> 0
// 1 2 -> 1
// 2 3 -> 2


// if this->order < section:
//     nex index = section - 1