#include "instrument.h"
#include "global_config.h"
#include "midi.h"



SineSynth::SineSynth()
{
    // Generate notes A3 (57) to A4 (69)
    for (unsigned n = 57; n <= 61; n++)
    {
        printf("Sine @%f\n", GMidi::midi_note_to_freq(n));
        this->oscilators.push_back(Sine(GMidi::midi_note_to_freq(n)));
    }

    for (auto osc : this->oscilators)
    {
        osc.turn_off();
    }

    
}




void
SineSynth::handle_event(MidiEvent & event)
{
    unsigned index = event.get_note_num() - 57;
    if (index < this->oscilators.size())
    {

        if (event.is_note_pressed())
        {
            this->oscilators[index].turn_on();
        }
        else
        {
            this->oscilators[index].turn_off();
        }
    }else{
        // printf("INDEX OOR\n");
    }
    
}

void 
SineSynth::render(Frame & frame)
{
    auto frame_size = GConfig::get_instance().get_frames_per_buffer();
    auto channels   = GConfig::get_instance().get_num_channels();

    for (int i = 0; i < frame_size; i++)
    {
        float sample = this->get_next_sample();
        for (int c = 0; c < channels; c++)
        {
            frame(c,i) = sample;
        }
    }
}

float
SineSynth::get_next_sample()
{
    float out = 0.0f;
    for (Sine & osc : this->oscilators)
    {
        //if (osc.on) {
            out += osc.get_next_sample();
        //}
    }
    return out;
}