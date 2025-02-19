#include "midi.h"
#include "instrument.h"

float 
GMidi::midi_note_to_freq(unsigned midi_note) 
{
    constexpr float A4_FREQ = 440.0f;
    constexpr float A4_MIDI_NUM = 69.0f;
    constexpr float SEMITONES_PER_OCTAVE = 12.0f;
    return A4_FREQ * std::powf(2.0f, (midi_note - A4_MIDI_NUM) / SEMITONES_PER_OCTAVE);
}

void 
GMidi::create_global_event(unsigned note_num, bool is_pressed) 
{
    auto event = MidiEvent(is_pressed, note_num);
    for (auto i : this->active_instruments)
    {
        i.second->handle_event(event);
    }

}

void 
GMidi::activate_instrument(std::shared_ptr<SineSynth> i, std::string name)
{
    this->active_instruments[name] = i;
}