#ifndef __GUI_H__
#define __GUI_H__

#include <vector>
#include "../error.h"

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


    std::vector<WayfarerGuiComp *> gui_comps;

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

    void register_comp(WayfarerGuiComp * c)
    {
        if (c == nullptr)
        {
            critical_error("Gui Component passed as NULL\n");
        }
        gui_comps.push_back(c);
    }


};

#endif // __GUI_H__