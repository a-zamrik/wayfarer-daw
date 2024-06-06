#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <unordered_map>
#include <string>
#include <functional>
#include "global_config.h"

#ifdef _WIN32

enum VK_MAP
{
    A = 0x41,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z
};

#endif


class Controller
{
protected:
    std::unordered_map<int, bool> keybindings_state; 
    std::unordered_map<int, std::function<void()>> keybindings; 
    std::unordered_map<int, unsigned> midi_bindings; 
    unsigned midi_octave_offset = 0;

public:

    void tick();

};

class KeyboardController : public Controller
{
private:
    void build_keyboard();

public:
    KeyboardController();
    void add_key_bind(int, std::function<void()>);
    void octave_shift_up() {this->midi_octave_offset++; this->build_keyboard();}
    void octave_shift_down() {this->midi_octave_offset--; this->build_keyboard();}
};



class GControllers
{
    private:
        GControllers() {}                        // Constructor? (the {} brackets) are needed here.

        std::vector<Controller> controllers;

        uint32_t sample_count = 0;
        uint32_t update_freq  = 100; //update 10 times a second

    public:
        static GControllers& get_instance()
            {
                static GControllers    instance; // Guaranteed to be destroyed.
                                            // Instantiated on first use.
                return instance;
            }
        
        GControllers(GControllers const&)         = delete;
        void operator=(GControllers const&)  = delete;

        void register_controller(Controller & cont)
        {
            this->controllers.push_back(cont);
        }

        void tick(uint32_t samples_passed)
        {
            this->sample_count += samples_passed;
            if (this->sample_count >= (GConfig::get_instance().get_sample_rate() / this->update_freq))
            {
                for (auto c : this->controllers)
                {
                    c.tick();
                }

                this->sample_count = 0;
            }
        }

};


#endif
