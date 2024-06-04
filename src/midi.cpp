#include "midi.h"

inline float 
GMidi::midi_note_to_freq(unsigned midi_note) 
{
    constexpr float A4_FREQ = 440.0f;
    constexpr float A4_MIDI_NUM = 69.0f;
    constexpr float SEMITONES_PER_OCTAVE = 12.0f;
    return A4_FREQ * std::powf(2.0f, (midi_note - A4_MIDI_NUM) / SEMITONES_PER_OCTAVE);
}

void 
GMidi::create_event(unsigned note_num, bool is_pressed) 
{
    this->event_queue.emplace(new MidiEvent(is_pressed, note_num));
}
