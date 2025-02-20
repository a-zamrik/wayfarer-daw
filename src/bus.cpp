#include "bus.h"
#include "error.h"
#include "global_config.h"
#include "controller.h"
#include "instrument.h"
#include <iostream>
#include <algorithm>
#include <inttypes.h>

#include "loader/wave.h"
#include "util/linked_list.h"


void 
Bus::render_frame(Frame & frame)
{


    unsigned n_channels = GConfig::get_instance().get_num_channels();

    bool need_update_gui = false;

    // Sin generotor populate frame
    this->synth->render(frame);

    this->audio_track->fill_frame(frame);

    this->effects_lock.lock();
    for (auto it = this->effects.begin(); it != this->effects.end(); it++)
    {
        (*it)->filter_frame(frame);

        // Gui has a list of weak pointers, it will remove its copy if it is delted
        if ((*it)->deleted)
        {
            it = this->effects.erase(it);
            this->update_chain_oder();
            if (it == this->effects.end())
            {
                // If we are at the end, we can't incrment
                break;
            }
        }
    }
    this->effects_lock.unlock();

    
    if (need_update_gui)
    {
        std::vector<std::weak_ptr<WayfarerGuiComp>> _gui_effects;
        _gui_effects.push_back(std::weak_ptr<WayfarerGuiComp>(this->synth));

        for (auto it = this->effects.begin(); it != this->effects.end(); it++)
        {
            _gui_effects.push_back(std::weak_ptr<WayfarerGuiComp>(*it));
        }

        this->gui_effects_update_q.push(_gui_effects);
    }

    return;
}

int 
MasterBus::paCallback( 
    const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData )
{

    // Really should not be printing here, way too slow
    if (statusFlags & paOutputUnderflow)
    {
        printf("underflow\n");
    }
    if (statusFlags & paOutputOverflow)
    {
        printf("overflow, no room for data\n");
    }

    unsigned n_channels = GConfig::get_instance().get_num_channels();

    float *out = (float*)outputBuffer;
    MasterBus * MasterBus =  static_cast<class MasterBus*>(userData);


    MasterBus->busses_lock.lock();
    for (auto it = MasterBus->busses.begin(); it != MasterBus->busses.end(); it++)
    {
        (*it)->render_frame(MasterBus->frame);
    }
    MasterBus->busses_lock.unlock();

    // Pass frame info to output 
    for (unsigned i = 0; i < framesPerBuffer; i++)
    {
        for (unsigned c = 0; c < n_channels; c++)
        {
            *out++ = MasterBus->gain * MasterBus->frame(c,i); 
            MasterBus->frame(c,i) = 0; // clear frame
        }
    }

    return paContinue;
}

void
MasterBus::init_stream()
{
    PaError err;

    PaStreamParameters outputParameters;

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        critical_error_no_line_print("Default device not found");
    }

    const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(outputParameters.device);
    if (pInfo != 0)
    {
        printf("Output device name: '%s'\n", pInfo->name);
    }

    outputParameters.channelCount = GConfig::get_instance().get_num_channels();       
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
        &stream,
        NULL, /* no input */
        &outputParameters,
        GConfig::get_instance().get_sample_rate(),
        GConfig::get_instance().get_frames_per_buffer(),
        paClipOff,      /* we won't output out of range samples so don't bother clipping them */
        &MasterBus::paCallback,
        this            
    );

    if (err != paNoError)
    {
        std::cerr << Pa_GetErrorText(err) << std::endl;
        critical_error_no_line_print("Failed to create audio stream");
    }

    return;
}

void
MasterBus::start_stream()
{
    PaError err = Pa_StartStream( stream );
    if (err != paNoError) {
        printf("stream failed to start\n");
        std::cerr << Pa_GetErrorText(err) << std::endl;
        critical_error_no_line_print("Failed to start audio stream");
    }
}

void
MasterBus::stop_stream()
{
    PaError err = Pa_StopStream( stream );
    if (err != paNoError) {
        printf("stream stop failed\n");
        std::cerr << Pa_GetErrorText(err) << std::endl;
        critical_error_no_line_print("Failed to stop audio stream");
    }
}


Bus& 
Bus::set_gain(float _gain) 
{
    constexpr float MAX_GAIN = 0.05f;
    this->gain = std::max(std::min(_gain, MAX_GAIN), 0.0f);

    return *this;
}

void
Bus::add_effect_at(uint64_t effect_id, uint32_t index)
{
    if (effect_id == 1)
    {

        // Rebuild effect list
        this->effects_lock.lock();
        this->effects.insert(std::shared_ptr<AutoFilter> (new AutoFilter(0.707f, 1000.f)), index);
        this->update_chain_oder();
        this->effects_lock.unlock();


        // build new list for gui
        std::vector<std::weak_ptr<WayfarerGuiComp>> _gui_effects;
        _gui_effects.push_back(std::weak_ptr<WayfarerGuiComp>(this->synth));

        for (auto it = this->effects.begin(); it != this->effects.end(); it++)
        {
            _gui_effects.push_back(std::weak_ptr<WayfarerGuiComp>(*it));
        }

        this->gui_effects_update_q.push(_gui_effects); 
    }
}

void
Bus::move_effect_to(uint64_t src_idx, uint32_t dest_idx)
{

    //printf("Moving %" PRIu64"  to %u\n", src_idx, dest_idx);

    auto new_effect =  std::shared_ptr<AutoFilter> (new AutoFilter(0.707f, 1000.f));

    auto it_src =  this->effects.begin();
    it_src += (unsigned) src_idx;

    // Rebuild effect list
    this->effects_lock.lock();
    this->effects.move_to_index(it_src, dest_idx);
    this->update_chain_oder();
    this->effects_lock.unlock();

    // build new list for gui
    std::vector<std::weak_ptr<WayfarerGuiComp>> _gui_effects;
    _gui_effects.push_back(std::weak_ptr<WayfarerGuiComp>(this->synth));

    for (auto it = this->effects.begin(); it != this->effects.end(); it++)
    {
        _gui_effects.push_back(std::weak_ptr<WayfarerGuiComp>(*it));
    }

    this->gui_effects_update_q.push(_gui_effects); 

}


#ifdef USE_IMGUI

#include "imgui.h"
#include "imgui_internal.h"


void Bus::draw_gui()
{
    // Need to update our gui list of effects
    this->gui_effects_update_q.try_pop(gui_effects);

    static bool showInstrumentChain = true;
    ImGui::Begin("Instrument Chain", &showInstrumentChain);                          // Create a window called "Hello, world!" and append into it.

    int seperator_id = 0;
    for(auto it = this->gui_effects.begin(); it != this->gui_effects.end(); it++)
    {    
        
        if (std::shared_ptr<WayfarerGuiComp> sp_c = it->lock()) {
            sp_c->draw_gui(); // If the effect still exists, draw it
        }
        else {
            it = this->gui_effects.erase(it); 
            if (it == this->gui_effects.end())
            {
                // If we are at the end, we can't incrment
                break;
            }
        }

        ImGui::SameLine();
        {   // Draw line seperator
            ImGui::PushID(seperator_id);
            ImGui::SameLine();
            ImGui::InvisibleButton("seperator", ImVec2(5,150));

            if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
                    {

                        IM_ASSERT(payload->DataSize == sizeof(int));
                        int payload_n = *(const int*)payload->Data;
                        //printf("Recieved %d as payload\n", payload_n);

                        this->add_effect_at(payload_n, seperator_id);
                    }

                    
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Effect_Move"))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(int));
                        int effect_order_num = *(const int*)payload->Data;
                        //printf("Effect %d as wants to move\n", effect_order_num);

                        if (effect_order_num < seperator_id)
                            this->move_effect_to(effect_order_num, seperator_id - 1);
                        else
                            this->move_effect_to(effect_order_num, seperator_id);
                    }
                    ImGui::EndDragDropTarget();
                }


            ImGui::DebugDrawItemRect(IM_COL32_WHITE);
            ImGui::SameLine();
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRect(p, ImVec2(p.x + 3, p.y + 150), IM_COL32(0,255,252, 255) , 5.0f);
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(2, 0));
            ImGui::SameLine();
            ImGui::PopID();
            seperator_id++;
        }
    }

    ImGui::End();
}

#endif