#ifndef __GUI_H__
#define __GUI_H__

class WayfarerGUI
{
private:
    WayfarerGUI();  // initalizes GUI
    ~WayfarerGUI();  // initalizes GUI

    bool show_demo_window = false;
    bool show_another_window = false;
    bool done = false;
    bool show_hello_world = true;


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
};

#endif // __GUI_H__