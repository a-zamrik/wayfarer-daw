#ifndef _MIDI_H_
#define _MIDI_H_

#include <queue>
#include <memory>
#include <cmath>
#include <unordered_map>
#include <string>
#include "instrument.h"





class MidiEvent
{

private:
    bool     is_pressed;
    unsigned note_num;

public:

    MidiEvent(bool _is_pressed, unsigned _note_num) : is_pressed(_is_pressed), note_num(_note_num) {}

    // TODO: Should we create midi events? or just have a global array that can be read from
    inline bool     is_note_pressed() {return this->is_pressed;}
    inline unsigned get_note_num()    {return this->note_num;}
};


// Agent between a controller (like keyboard controller) to an active instrument
class GMidi
{
    private:
        GMidi() {}                        // Constructor? (the {} brackets) are needed here.


        std::unordered_map<std::string, std::shared_ptr<SineSynth>> active_instruments;

    public:
        static GMidi& get_instance()
            {
                static GMidi    instance; // Guaranteed to be destroyed.
                                            // Instantiated on first use.
                return instance;
            }
        
        GMidi(GMidi const&)           = delete;
        void operator=(GMidi const&)  = delete;


        static float midi_note_to_freq(unsigned midi_note);

        void create_global_event(unsigned note_num, bool is_pressed);

        void activate_instrument(std::shared_ptr<SineSynth>  i, std::string name);

};

#endif