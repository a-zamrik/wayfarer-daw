#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_


#include "oscillators.h"
#include <vector>

class MidiEvent;

class Instrument
{

  protected: 
    

  public:


    virtual void handle_event(MidiEvent & event) = 0;    
    virtual void render(Frame & frame) = 0;
};

class SineSynth : public Instrument
{
    private:
        std::vector<Sine> oscilators;


    public:
        SineSynth();
        virtual void handle_event(MidiEvent & event);    
        virtual void render(Frame & frame);

        float get_next_sample();
};

#endif