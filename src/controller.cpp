#include "controller.h"
#include "midi.h"

#ifdef _WIN32
#include <windows.h>

void
Controller::tick()
{
    for (auto kb : this->keybindings)
    {
        if(GetKeyState(kb.first) & 0x8000 /*Check if high-order bit is set (1 << 15)*/)
        {
            kb.second();
        }
    }

    for (auto mb : this->midi_bindings)
    {
        if(GetKeyState(mb.first) & 0x8000 /*Check if high-order bit is set (1 << 15)*/)
        {
            //printf("%u PRESSED\n", mb.second);
            GMidi::get_instance().create_event(mb.second, true);
        }
        else
        {
            GMidi::get_instance().create_event(mb.second, false);
        }
    }
}

KeyboardController::KeyboardController()
{
    this->build_keyboard();
}

void
KeyboardController::build_keyboard()
{
    // key roll starting form midi note C mapped to 'A key' keayboard
    this->midi_bindings[0x41] = 72u + (this->midi_octave_offset * 12); //a
    this->midi_bindings[0x57] = 73u + (this->midi_octave_offset * 12); //w
    this->midi_bindings[0x53] = 74u + (this->midi_octave_offset * 12); //s
    this->midi_bindings[0x45] = 75u + (this->midi_octave_offset * 12); //e
    this->midi_bindings[0x44] = 76u + (this->midi_octave_offset * 12); //d
    this->midi_bindings[0x46] = 77u + (this->midi_octave_offset * 12);  //f
    this->midi_bindings[0x54] = 78u + (this->midi_octave_offset * 12);  //t
    this->midi_bindings[0x47] = 79u + (this->midi_octave_offset * 12);  //g
    this->midi_bindings[0x59] = 80u + (this->midi_octave_offset * 12);  //y
    this->midi_bindings[0x48] = 81u + (this->midi_octave_offset * 12);  //h
    this->midi_bindings[0x55] = 82u + (this->midi_octave_offset * 12);  //h
    this->midi_bindings[0x4A] = 83u + (this->midi_octave_offset * 12);  //h
}


void
KeyboardController::add_key_bind(int key, std::function<void()> func)
{
    this->keybindings[key] = func;
}

#endif