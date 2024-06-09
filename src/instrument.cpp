#include "instrument.h"
#include "global_config.h"
#include "midi.h"


#define START_MIDI_NOTE 21
#define END_MIDI_NOTE 108


float
linear_to_db(float float_pcm)
{
    float abs = std::abs(float_pcm);
    if (abs <= 0)
    {
        return -144.0f; // -inf db essentially
    }
    else
    {
        return 20.0f * log10f(abs);
    }
}

float
db_to_linear(float float_db)
{
    return powf(10.0f, float_db / 20.0f);
}
    

SineSynth::SineSynth()
{

    printf("CREATING SYNTH\n");

    for (unsigned n = START_MIDI_NOTE; n <= END_MIDI_NOTE; n++)
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
    unsigned index = event.get_note_num() - START_MIDI_NOTE;
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
    }
    else
    {
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
        float sample = this->get_next_sample() * this->gain_lin;
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
        // Only oscilators that have samples to output should be invoked
        if (osc.is_outputing()) {
            out += osc.get_next_sample();
        }
    }
    return out;
}


#ifdef USE_IMGUI
#include "imgui.h"
#include "imgui-knobs.h"

void
SineSynth::draw_gui()
{
    ImGui::TextColored(ImVec4(0.0f, 255.0f, 252.0f, 0.8f), "Sine Synth");
    if (ImGuiKnobs::Knob("Gain", &this->gain_db, -60.0f, 6.0f, 0.1f, "%.1fdB", ImGuiKnobVariant_Wiper)) {
        this->gain_lin = db_to_linear(this->gain_db);
    }
}
#endif