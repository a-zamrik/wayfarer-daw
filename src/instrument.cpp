#include "instrument.h"
#include "global_config.h"
#include "midi.h"
#include "sequence.h"


#define START_MIDI_NOTE 21
#define END_MIDI_NOTE 108



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
        printf("MIDI NOTE OOR // INDEX OOR\n");
    }
    
}

void 
SineSynth::render(Frame & frame)
{
    auto frame_size = GConfig::get_instance().get_frames_per_buffer();
    auto channels   = GConfig::get_instance().get_num_channels();

    for (int i = 0; i < frame_size; i++)
    {
        if (auto b_seq = bound_sequence.lock())
        {
            // TODO: this is convoluted, we should have tick return a midi event intead
            b_seq->tick();
        }

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

void
SineSynth::update_adsr()
{
    for (Sine & osc : this->oscilators)
    {
        osc.adsr_env.set_env(attack_time, attack_amp, decay_time, release_time, sustain_amp);
    }
}


#ifdef USE_IMGUI
#include "imgui.h"
#include "imgui-knobs.h"

void
SineSynth::draw_gui()
{
    float plot_values[] = { 0.0f, 1.0f, 0.5f, 0.5, 0.25, 0.0f};

    //ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(12, 0, 64, 255));

    // TODO: need to us pushID and use instance IDs

    ImGui::BeginChild("SinInstrument", ImVec2(575, 155), 0, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
        ImGui::TextColored(ImVec4(0.0f, 255.0f, 252.0f, 0.8f), "Sine Synth");

        // TODO: add ADSR editor
        ImGui::BeginChild("##ADSR", ImVec2(500, 150), 0);
            bool update_adsr = false;
            ImGui::PlotLines("", plot_values, IM_ARRAYSIZE(plot_values), 0, "ADSR", 0, 1.0f, ImVec2(500, 50.0f));
            if (ImGuiKnobs::Knob("Atk", &this->attack_time, -0, 20.0f, 0.01f, "%.2f", ImGuiKnobVariant_Wiper, 35.0f)) {
                update_adsr = true;
            }
            ImGui::SameLine();
            if (ImGuiKnobs::Knob("Dec", &this->decay_time, 0, 20.0f, 0.01f, "%.2f", ImGuiKnobVariant_Wiper, 35.0f)) {
                update_adsr = true;
            }
            ImGui::SameLine();
            if (ImGuiKnobs::Knob("A A", &this->attack_amp, 0.0f, 1.0f, 0.01f, "%.2f", ImGuiKnobVariant_Wiper, 35.0f)) {
                update_adsr = true;
            }
            ImGui::SameLine();
            if (ImGuiKnobs::Knob("S A", &this->sustain_amp, 0.0f, 1.0f, 0.01f, "%.2f", ImGuiKnobVariant_Wiper, 35.0f)) {
                update_adsr = true;
            }
            ImGui::SameLine();
            if (ImGuiKnobs::Knob("Rel", &this->release_time, 0.0f, 20.0f, 0.01f, "%.2f", ImGuiKnobVariant_Wiper, 35.0f)) {
                update_adsr = true;
            }

            if (update_adsr)
            {
                this->update_adsr();
            }
        ImGui::EndChild();

        // Draw seprator
        ImGui::SameLine();
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + 3, p.y + 150), IM_COL32(0,155,155, 255) , 5.0f);
        ImGui::SameLine();
        ImGui::Dummy(ImVec2(2, 0));
        ImGui::SameLine();

        // Draw Output section
        ImGui::BeginChild("##Gain", ImVec2(35, 140), 0);
        if (ImGuiKnobs::Knob("Gain", &this->gain_db, -60.0f, 6.0f, 0.1f, "%.1fdB", ImGuiKnobVariant_Wiper, 35.0f)) {
            this->gain_lin = db_to_linear(this->gain_db);
        }
        ImGui::EndChild();
    ImGui::EndChild();

    //ImGui::PopStyleColor();

}
#endif