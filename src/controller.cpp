#include "controller.h"

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
}

void
KeyboardController::add_key_bind(int key, std::function<void()> func)
{
    this->keybindings[key] = func;
}

#endif