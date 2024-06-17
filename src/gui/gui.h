#ifndef __GUI_H__
#define __GUI_H__

#include <vector>
#include "../error.h"
#include <memory>

class WayfarerGuiComp
{
public:
    virtual void draw_gui() {}
};


class WayfarerGUI
{
private:
    WayfarerGUI();  // initalizes GUI
    ~WayfarerGUI();  // initalizes GUI

    bool show_demo_window = false;
    bool show_another_window = false;
    bool done = false;
    bool show_hello_world = true;


    std::vector<std::weak_ptr<WayfarerGuiComp>> gui_comps;

public:
    static WayfarerGUI& get_instance()
        {
            static WayfarerGUI instance; // Guaranteed to be destroyed.
                                         // Instantiated on first use.
            return instance;
        }
    
    WayfarerGUI(WayfarerGUI const&)     = delete;
    void operator=(WayfarerGUI const&)  = delete;

    // returns false if we should shutdown
    bool update_gui();
    void cleanup();

    void register_comp(std::weak_ptr<WayfarerGuiComp> c)
    {
        gui_comps.push_back(c);
    }
};

#endif // __GUI_H__