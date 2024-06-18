#include "controller.h"
#include "midi.h"

#ifdef _WIN32
#include <windows.h>

void
Controller::tick()
{
    // Process keybindings
    for (auto kb : this->keybindings)
    {
        if(GetAsyncKeyState(kb.first) & 0x8000 /*Check if high-order bit is set (1 << 15)*/)
        {
            // Only if the key was just pressed to we invoke the mapped function
            if (!this->keybindings_state[kb.first])
            {
                this->keybindings_state[kb.first] = true;
                kb.second();
            }
        }
        else
        {
            this->keybindings_state[kb.first] = false;
        }
    }

    // Process midi keys
    for (auto mb : this->midi_bindings)
    {
        if(GetAsyncKeyState(mb.first) & 0x8000 /*Check if high-order bit is set (1 << 15)*/)
        {
            if (!midi_states[mb.first])
            {
                midi_states[mb.first] = true;
                GMidi::get_instance().create_global_event(mb.second, true);
            }
        }
        else
        {
            if (midi_states[mb.first])
            {
                midi_states[mb.first] = false;
                GMidi::get_instance().create_global_event(mb.second, false);
            }
        }
    }
}

KeyboardController::KeyboardController()
{
    this->build_keyboard();

    for (int i = 0; i < 155; i++)
    {
        midi_states[i] = false;
    }
}

void
KeyboardController::build_keyboard()
{
    // key roll starting form midi note C mapped to 'A key' keayboard
    this->midi_bindings[VK_MAP::A] = 72u + (this->midi_octave_offset * 12); //a
    this->midi_bindings[VK_MAP::W] = 73u + (this->midi_octave_offset * 12); //w
    this->midi_bindings[VK_MAP::S] = 74u + (this->midi_octave_offset * 12); //s
    this->midi_bindings[VK_MAP::E] = 75u + (this->midi_octave_offset * 12); //e
    this->midi_bindings[VK_MAP::D] = 76u + (this->midi_octave_offset * 12); //d
    this->midi_bindings[VK_MAP::F] = 77u + (this->midi_octave_offset * 12);  //f
    this->midi_bindings[VK_MAP::T] = 78u + (this->midi_octave_offset * 12);  //t
    this->midi_bindings[VK_MAP::G] = 79u + (this->midi_octave_offset * 12);  //g
    this->midi_bindings[VK_MAP::Y] = 80u + (this->midi_octave_offset * 12);  //y
    this->midi_bindings[VK_MAP::H] = 81u + (this->midi_octave_offset * 12);  //h
    this->midi_bindings[VK_MAP::U] = 82u + (this->midi_octave_offset * 12);  //h
    this->midi_bindings[VK_MAP::J] = 83u + (this->midi_octave_offset * 12);  //h
}


void
KeyboardController::add_key_bind(int key, std::function<void()> func)
{
    this->keybindings[key] = func;
    this->keybindings_state[key] = false;
}

#endif