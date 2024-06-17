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



    for (uint32_t i = 60; i < MAX_MIDI; i++)
    {
        add_note(i, 3.0f + (0.5f * (i - 60)), 0.1f);
    }
    // add_note(60, 3.0f, 0.1f);
    // add_note(61, 3.5f, 0.1f);
    // add_note(62, 4.f, 0.1f);
    // add_note(60, 4.5f, 0.1f);

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
    // TODO: this needs to insert into the right place
    piano_roll[midi_note_num]->push_back(MidiSequence::Note(start_time_s, duration_s));
}


void
MidiSequence::tick()
{
    sample_index++;
    if (sample_index >= 48000 * 10)
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