#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include "util/linked_list.h"
#include <vector>
#include <memory>
#include "instrument.h"

// May not want this, just do some math based on the bottom time signature number
// and the passed in note type
//
// enum NoteType 
// {
//     WHOLE     = 1,
//     HALF      = 2,
//     QUARTER   = 4,
//     EIGTH     = 8,
//     SIXTEENTH = 16
// };


class GSequenceMaster
{
    /*
    *    Contianer where all sequences are registered to.
    *    Allows us to play / pause / and jump in time for all sequences.
    *    Also allows us to draw gui for each bus
    */
};

struct Note
    {
        // At what time t (in seconds) is the note pressed
        float start_s;
        
        // How long the note is in seconds
        float duration_s;

        Note(float start_time, float duraiton) : start_s(start_time), duration_s(duraiton) {}
        Note() : start_s(0), duration_s(0) {}
    };
    
class MidiSequence
{

private: 

    

    size_t sample_index;

    float update_period_ms = 1.0;  // how frequenlty should we check check for midin note changes
    uint32_t update_n_samples;     // how many samples need to pass before we cheack for midi not chagnes
    
   

    // Keeps track if the note has been pressed or not, need to know when to trigger a midi event
    std::vector<bool> note_state;

    // Keep track of where we are in notes sequence
    std::vector<LinkedList<Note>::Iterator> note_its;

public:
    // Map midi note number to the list of note events for that note
    // piano_roll[A4] -> list of A4 notes in order of when they are pressed
    //
    // THE LINKED LIST OF NOTES MUST BE SORTED IN ORDER OF START_TIME!!!!
    std::vector<
        std::shared_ptr<
            LinkedList<Note>
        >
    > piano_roll;

    // Bounded instrument
    std::weak_ptr<SineSynth> bound_instrument;

    MidiSequence();
    
    /*  Add a note to this sequence. This may remove/modify notes if the note added overlaps pervious notes.
    *
    *   start_time_s  => At what time t (in seconds) is the note pressed 
    *   duration_s    => How long the note is pressed in seconds
    *   midi_note_num => which midi key to push
    */
    void add_note(uint32_t midi_note_num, float start_time_s, float duration_s);

    // TODO: need a way to convert start time s and duration to quarter/8th/half/ and whole notes
    //       need to be able to specify which measure and beat they should be placed it
    //
    // void add_note(uint32_t midi_note_num, 
    //               uint32_t measure, 
    //               uint32_t beat, 
    //               uint32_t type)


    // Tell sequencer to move forward in time by a single sample
    void tick();

    void reset();

    /*
    *   move this sequence play needle to be at time_s where time_s is the time in seconds.
    */
    void set_time(float time_s);

};

#endif