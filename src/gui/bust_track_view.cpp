#include "gui.h"
#include "../error.h"
#include <stdio.h>
#include "bus_track_view.h"
#include "../sequence.h"
#include <cstdint>
#include <vector>

// Defined by CMAKE PREPROCESSING
#if defined(USE_IMGUI)
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui-knobs.h"

constexpr uint32_t BUS_INTERFACE_WIDTH  = 150;
constexpr uint32_t BUS_INTERFACE_HEIGHT = 100;
constexpr uint32_t HEADER_HEIGHT = 30;


// TODO: https://github.com/alanjfs/sequentity


void draw_header()
{
    auto screen_pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddRectFilled(screen_pos, ImVec2(screen_pos.x + ImGui::GetWindowSize().x, screen_pos.y + HEADER_HEIGHT), IM_COL32(0, 255, 255, 255), 0.0f, 0);

    // TODO: Draw measures / time tick marks

    ImGui::SetCursorScreenPos(ImVec2(screen_pos.x, screen_pos.y + HEADER_HEIGHT));
}

void draw_bus_track(uint32_t track_id, std::shared_ptr<Bus> & _bus)
{
    auto screen_pos = ImGui::GetCursorScreenPos();
    uint32_t track_width = ImGui::GetWindowSize().x - BUS_INTERFACE_WIDTH;
    ImGui::GetWindowDrawList()->AddRectFilled(screen_pos, ImVec2(screen_pos.x + track_width, screen_pos.y + 300), IM_COL32(255, 255, 255, 255), 0.0f, 0);

    for (std::shared_ptr<LinkedList<Note>> & key_list : _bus->midi_sequence->piano_roll)
    {
        int i = 0;
        for (Note & note : (*key_list))
        {
            ImGui::GetWindowDrawList()->AddRectFilled(screen_pos, ImVec2(screen_pos.x + (note.start_s + 10) + i, screen_pos.y + (note.duration_s + 10)), IM_COL32(255, 0, 255, 255), 0.0f, 0);

        }
        i += 500;
    }

}

void 
GuiBusTrackView::draw_gui()
{
    static bool show_track_view = true;


    ImGui::Begin("Track/Bus View", &show_track_view);
    draw_header();

    // Draw track view
    int i = 0;
    for ( auto bus : this->busses)
    {
        ImGui::PushID(i);
        
        ImGui::Button("Bus");
        draw_bus_track(i, bus);
        ImGui::PopID();
        i++;
    }

    ImGui::End();
}

#else

void 
GuiBusTrackView::draw_gui()
{
    critical_error("Compiled without a GUI framework, WayfarerGuicomponents are not functional");
}

#endif