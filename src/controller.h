#ifndef CONTROLLER_H
#define CONTROLLER_H


#include <unordered_map>
#include <string>
#include <functional>
#include "global_config.h"


class Controller
{
protected:
    std::unordered_map<int, std::function<void()>> keybindings; 

public:

    virtual void add_key_bind(int, std::function<void()>) {};
    void tick();

};

class KeyboardController : public Controller
{

public:

    virtual void add_key_bind(int, std::function<void()>);
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
