#include "filters.h"
#include "../oscillators.h"
#include "../global_config.h"
#include "../util/wmath.h"
#include <cmath>
#include <iostream>

uint32_t AutoFilter::instance_count = 0;

void
BiQuadFilter::filter_frame(Frame & frame)
{
    auto frame_size = GConfig::get_instance().get_frames_per_buffer();
    auto channels   = GConfig::get_instance().get_num_channels();

    for (int i = 0; i < frame_size; i++)
    {
        for (int c = 0; c < channels; c++)
        {
            frame(c,i) = this->filter(frame(c,i));
        }
    }
}

float
BiQuadFilter::filter(float sample)
{
    float y = (b0 * sample) + (b1 * x1) + (b2 * x2) - (a1 * y1) - (a2 * y2);
    x2 = x1;
    x1 = sample;
    y2 = y1;
    y1 = y;
    return y;
}

float 
BiQuadFilter::__freq_domain_at(float freq)
{
    float phi = pow(sin((TWO_PI_F * freq) / (2.0f * GConfig::get_instance().get_sample_rate())), 2.0f);
    float phi_sq = phi * phi;

    float numerator   = 16 * b0 * b2 * phi_sq + pow(b0 + b1 + b2, 2.0f) - 4 * (b0 * b1 + 4*b0*b2 + b1*b2) * phi;
    float denominator = 16 *a2*phi_sq + pow(a1 + a2 + 1, 2.f) - 4*(a1*a2 + a1 + 4*a2) * phi;

    return numerator / denominator;
}

void
BiQuadFilter::__populate_freq_domain_table(float *array, uint32_t count)
{
    float s = this->min_freq;
    float freq_step = (this->max_freq - this->min_freq) / ((float) count);
    for (uint32_t i = 0; i < count; i++)
    {
        array[i] = this->__freq_domain_at(logspace(this->min_freq, this->max_freq, i, count));

        //printf("array[%d] = %f\n", i, array[i]);
    }
}

void
AutoFilter::__make_lowpass()
{
    // TODO:
    // sample_rate, max_freq, omega, and alpha are duplicate code
    // for all types of filters, could calculate before calling the
    // __make_xxxxx() functions
    
    auto sample_rate = GConfig::get_instance().get_sample_rate();

    this->max_freq = (sample_rate - 100.0f) / 2.0f;

    float omega = TWO_PI_F * this->center_freq / sample_rate;
    float alpha = sin(omega) / (2.0f * this->q);

    
    
    this->b0 = (1.0f - cos(omega)) / 2.0f;
    this->b1 = (1.0f - cos(omega));
    this->b2 = (1.0f - cos(omega)) / 2.0f;

    this->a0 = 1.0f + alpha;
    this->a1 = -2.0f * cos(omega);
    this->a2 = 1.0f - alpha;

    this->__normalize_coefficients();
}

void
AutoFilter::__make_highpass()
{
    auto sample_rate = GConfig::get_instance().get_sample_rate();

    this->max_freq = (sample_rate - 100.0f) / 2.0f;

    float omega = TWO_PI_F * this->center_freq / sample_rate;
    float alpha = sin(omega) / (2.0f * this->q);
    
    this->b0 = (1.0f + cos(omega)) / 2.0f;
    this->b1 = -(1.0f + cos(omega));
    this->b2 = (1.0f + cos(omega)) / 2.0f;

    this->a0 = 1.0f + alpha;
    this->a1 = -2.0f * cos(omega);
    this->a2 = 1.0f - alpha;

    this->__normalize_coefficients();
}

void
AutoFilter::__make_bandpass()
{
    auto sample_rate = GConfig::get_instance().get_sample_rate();

    this->max_freq = (sample_rate - 100.0f) / 2.0f;

    float omega = TWO_PI_F * this->center_freq / sample_rate;
    float alpha = sin(omega) / (2.0f * this->q);
    
    this->b0 = alpha;
    this->b1 = 0.f;
    this->b2 = -alpha;

    this->a0 = 1.0f + alpha;
    this->a1 = -2.0f * cos(omega);
    this->a2 = 1.0f - alpha;

    this->__normalize_coefficients();
}

void
AutoFilter::__make_notch()
{
    auto sample_rate = GConfig::get_instance().get_sample_rate();

    this->max_freq = (sample_rate - 100.0f) / 2.0f;

    float omega = TWO_PI_F * this->center_freq / sample_rate;
    float alpha = sin(omega) / (2.0f * this->q);
    
    this->b0 = 1.f;
    this->b1 = -2.0f * cos(omega);
    this->b2 = 1.f;

    this->a0 = 1.0f + alpha;
    this->a1 = -2.0f * cos(omega);
    this->a2 = 1.0f - alpha;

    this->__normalize_coefficients();
}


void
AutoFilter::__normalize_coefficients()
{
    // For optimization, we can normalize a0 to be equal to 1
    // afterwhich, a division by 1, has no effect on result.
    this->b0 /= this->a0;
    this->b1 /= this->a0;
    this->b2 /= this->a0;
    this->a1 /= this->a0;
    this->a2 /= this->a0;
}

void
AutoFilter::__recalculate_coefficients()
{
    switch (this->filter_type)
    {
        case FilterType::LowPass:
            this->__make_lowpass();
            break;
        case FilterType::HighPass:
            this->__make_highpass();
            break;
        case FilterType::BandPass:
            this->__make_bandpass();
            break;
        case FilterType::Notch:
            this->__make_notch();
            break;
        default:
            critical_error("Some how the filter type was not known\n");
    }


    // std::cout << "b0:" << b0 << std::endl
    //           << "b1:" << b1 << std::endl
    //           << "b2:" << b2 << std::endl
    //           << "a1:" << a1 << std::endl
    //           << "a2:" << a2 << std::endl;
}


#ifdef USE_IMGUI
#include "imgui.h"
#include "imgui-knobs.h"


void
AutoFilter::draw_gui()
{
    bool update_params = false;

    ImGui::PushID(this->instance_id);
    

    ImGui::BeginGroup();

        ImGui::Button("AutoFilter");
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            // Set payload to carry the index of our item (could be anything)
            ImGui::SetDragDropPayload("Effect_Move", &this->chain_order, sizeof(int));

            // Display preview (could be anything, e.g. when dragging an image we could decide to display
            // the filename and a small preview of the image, etc.)
            ImGui::Text("AutoFilter"); 
            
            ImGui::EndDragDropSource();
        }
    
        //ImGui::TextColored(ImVec4(0.0f, 255.0f, 252.0f, 0.8f), "Filter");
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_NoSharedDelay))
            ImGui::SetTooltip("Right-Click Me");

        if (ImGui::BeginPopupContextItem("my popup"))
        {
            ImGui::Text("Options:");
            if (ImGui::Selectable("  Remove From Bus"))
            {
                printf("Trying to remove filter with id = %d from bus\n", this->instance_id);
                this->deleted = true;
            }
            ImGui::EndPopup();
        }
        
        

        ImGui::BeginGroup();
            ImGui::PlotLines("", this->__freq_response_table, IM_ARRAYSIZE(this->__freq_response_table), 0, "f-domian", 0, 5.0f, ImVec2(135, 110.0f));

            ImGui::Dummy(ImVec2(3, 0));
            ImGui::SameLine();
            if (ImGui::Button("LO"))
            {
                
                this->filter_type = FilterType::LowPass;
                update_params = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("HI"))
            {
                this->filter_type = FilterType::HighPass;
                update_params = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("BA"))
            {
                this->filter_type = FilterType::BandPass;
                update_params = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("NO"))
            {
                this->filter_type = FilterType::Notch;
                update_params = true;
            }
        ImGui::EndGroup();

        ImGui::SameLine();

        ImGui::BeginGroup();

            if (ImGuiKnobs::Knob("Q", &this->q, 0.01f, 10.0f, 0.01f, "%.2f", ImGuiKnobVariant_Wiper, 25.0f)) {
                update_params = true;
            }
            if (ImGuiKnobs::Knob("F", &this->center_freq, this->min_freq, this->max_freq, 100.0f, "%.2f", ImGuiKnobVariant_Wiper, 25.0f)) {
                update_params = true;
            }
        ImGui::EndGroup();
        
    ImGui::EndGroup();

    ImGui::PopID();
    if (update_params) 
    {
        this->__recalculate_coefficients();
        this->__populate_freq_domain_table(this->__freq_response_table, IM_ARRAYSIZE(this->__freq_response_table));
    }
        
}
#endif