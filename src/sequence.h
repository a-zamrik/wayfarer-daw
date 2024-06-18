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


class MidiSequence
{

private: 

    struct Note
    {
        // At what time t (in seconds) is the note pressed
        float start_s;
        
        // How long the note is in seconds
        float duration_s;

        Note(float start_time, float duraiton) : start_s(start_time), duration_s(duraiton) {}
        Note() : start_s(0), duration_s(0) {}
    };

    size_t sample_index;
    
    // Map midi note number to the list of note events for that note
    // piano_roll[A4] -> list of A4 notes in order of when they are pressed
    //
    // THE LINKED LIST OF NOTES MUST BE SORTED IN ORDER OF START_TIME!!!!
    std::vector<
        std::shared_ptr<
            LinkedList<Note>
        >
    > piano_roll;

    // Keeps track if the note has been pressed or not, need to know when to trigger a midi event
    std::vector<bool> note_state;

    // Keep track of where we are in notes sequence
    std::vector<LinkedList<Note>::Iterator> note_its;

public:
    // Bounded instrument
    std::weak_ptr<SineSynth> bound_instrument;

    MidiSequence();

    // TODO: Impliment loading a midi file
    
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

    // TODO: set sample_index to represent time.
    //       update key states and note iterators to reflect change
    void set_time(float time) { critical_error("NOT IMPLMENTED");}

};

#endif