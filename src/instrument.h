#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_


#include "oscillators.h"
#include "./gui/gui.h"
#include <vector>


float linear_to_db(float float_pcm);
float db_to_linear(float float_db);

class MidiEvent;

class Instrument
{

  protected: 
    

  public:


    virtual void handle_event(MidiEvent & event) = 0;    
    virtual void render(Frame & frame) = 0;
};

class SineSynth : public Instrument, public WayfarerGuiComp
{
    private:
        std::vector<Sine> oscilators;
        float gain_lin = 1.0;
        float gain_db  = 0.0;


    public:
        SineSynth();
        virtual void handle_event(MidiEvent & event);    
        virtual void render(Frame & frame);

        float get_next_sample();

#ifdef USE_IMGUI
        virtual void draw_gui();
#endif
};

#endif