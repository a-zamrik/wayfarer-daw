#ifndef __BUS_TRACK_VIEW_H__
#define __BUS_TRACK_VIEW_H__

#include "gui.h"
#include "../bus.h"
#include "../util/linked_list.h"

class GuiBusTrackView : public WayfarerGuiComp
{
    public:
    LinkedList<std::shared_ptr<Bus>> busses;
    std::shared_ptr<MasterBus>       master_bus;

    virtual void draw_gui();
};

#endif