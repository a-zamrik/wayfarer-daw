#include "sequence.h"
#include "global_config.h"
#include "midi.h"

constexpr int MAX_MIDI = 70;

MidiSequence::MidiSequence()
{
    sample_index = 0;

    for (int i = 0; i < MAX_MIDI; i++)
    {
        piano_roll.push_back(std::shared_ptr<LinkedList<Note>>(new LinkedList<Note>()));
        note_state.push_back(false);

    }



    // for (uint32_t i = 60; i < MAX_MIDI; i++)
    // {
    //     add_note(i, 3.0f + (0.5f * (i - 60)), 0.1f);
    // }

    add_note(60, 0.1f, 0.5f);
    add_note(60, 0.2f, 0.5f);
    add_note(60, 0.21f, 10.f);
    add_note(60, 0.f, 10.f);



    for (int i = 0; i < MAX_MIDI; i++)
    {
        note_its.push_back(piano_roll[i]->begin());
    }
}

void
MidiSequence::reset()
{
    sample_index = 0;
    for (int i = 0; i < MAX_MIDI; i++)
    {
        note_its[i] = piano_roll[i]->begin();
        note_state[i] = false;
    }
}

void
MidiSequence::add_note(uint32_t midi_note_num, float start_time_s, float duration_s)
{
    // Insert the note into the correct order
    LinkedList<Note>::Iterator it = piano_roll[midi_note_num]->sorted_insert(
        MidiSequence::Note(start_time_s, duration_s),
        [] (const Note & rhs, const Note & new_entry) -> bool {return rhs.start_s > new_entry.start_s;}
    );


    // TODO: This does not account for when more than one note before or after need to be adjusted

    // we now need trim the end time of the prvious note, and push back the start time of the next note after.
    if (--it != piano_roll[midi_note_num]->get_head())
    {
        if ((*it).duration_s > start_time_s)
        {
            (*it).duration_s = start_time_s;
        }
    }
    ++it; // now pointing at new entry

    // need to process as many notes after the new note as needed.
    // may be more than one note that needs to be deleted/adjusted
    while (++it != piano_roll[midi_note_num]->end() && (*it).start_s < start_time_s + duration_s)
    {
        
        // notes new start time goes further than its original end time, just remove it
        if ( ((*it).duration_s + (*it).start_s) < (start_time_s + duration_s) )
        {
            it = piano_roll[midi_note_num]->erase(it);
            --it; // erase returns the next element, but we incremnet in the while loop
        }
        // we can move the start time up, we have space. Duration adjusted to have the same end 
        else
        {
            (*it).duration_s -= (start_time_s + duration_s) - (*it).start_s;
            (*it).start_s    = start_time_s + duration_s;
        }
    }
    



}


void
MidiSequence::tick()
{
    sample_index++;
    if (sample_index >= 48000 * 20)
    {
        this->reset();
    }

    for (int i = 0; i < piano_roll.size(); i++)
    {
        // only process iterators that have more notes to consume
        if (note_its[i] != piano_roll[i]->end())
        {
            // If we just entered a notes start time, we need to trigger a midi on event.
            if (sample_index >= (*(note_its[i])).start_s * GConfig::get_instance().get_sample_rate())
            {
                // We entered the note, but the note has not been pressed yet
                if (note_state[i] == false)
                {
                    note_state[i] = true;
                    // GENERATE ON MIDI FOR KEY (i)
                    if(auto b_inst = bound_instrument.lock()) 
                    {
                        b_inst->handle_event(MidiEvent(true, i));
                    }
                    else
                    {
                        critical_error("INSTRUMENT BOUD TO THIS SEQUENCE DOES NOT EXIST ANYMORE");
                    }
                }
            }

            // If the note ends, we need to trigger a midi off event
            if (sample_index >= ((*(note_its[i])).start_s + (*(note_its[i])).duration_s)* GConfig::get_instance().get_sample_rate())
            {
                // We entered the note, but the note has not been pressed yet
                if (note_state[i] == true)
                {
                    note_state[i] = false;
                    // GENERATE OFF MIDI FOR KEY (i)
                    if(auto b_inst = bound_instrument.lock()) 
                    {
                        b_inst->handle_event(MidiEvent(false, i));
                    }
                    else
                    {
                        critical_error("INSTRUMENT BOUD TO THIS SEQUENCE DOES NOT EXIST ANYMORE");
                    }
                }

                // We have finished this note, move on to the next note
                note_its[i]++;
            }

        }
    }

}