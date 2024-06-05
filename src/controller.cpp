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
    // TODO: key roll starting form C mapped to 'A key'
    this->midi_bindings[0x41] = 57u; //a
    this->midi_bindings[0x53] = 58u; //s
    this->midi_bindings[0x44] = 59u; //d
    this->midi_bindings[0x46] = 60;   //f
}

void
KeyboardController::add_key_bind(int key, std::function<void()> func)
{
    this->keybindings[key] = func;
}

#endif