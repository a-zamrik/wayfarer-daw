// /** @file pa_devs.c
//     @ingroup examples_src
//     @brief List available devices, including device information.
//     @author Phil Burk http://www.softsynth.com

//     @note Define PA_USE_ASIO=0 to compile this code on Windows without
//         ASIO support.
// */
// /*
//  * $Id$
//  *
//  * This program uses the PortAudio Portable Audio Library.
//  * For more information see: http://www.portaudio.com
//  * Copyright (c) 1999-2000 Ross Bencina and Phil Burk
//  *
//  * Permission is hereby granted, free of charge, to any person obtaining
//  * a copy of this software and associated documentation files
//  * (the "Software"), to deal in the Software without restriction,
//  * including without limitation the rights to use, copy, modify, merge,
//  * publish, distribute, sublicense, and/or sell copies of the Software,
//  * and to permit persons to whom the Software is furnished to do so,
//  * subject to the following conditions:
//  *
//  * The above copyright notice and this permission notice shall be
//  * included in all copies or substantial portions of the Software.
//  *
//  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
//  * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
//  * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
//  * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//  */

// /*
//  * The text above constitutes the entire PortAudio license; however,
//  * the PortAudio community also makes the following non-binding requests:
//  *
//  * Any person wishing to distribute modifications to the Software is
//  * requested to send the modifications to the original developer so that
//  * they can be incorporated into the canonical version. It is also
//  * requested that these non-binding requests be included along with the
//  * license above.
//  */

// #include <stdio.h>
// #include <iostream>
// #include <math.h>
// #include "../include/portaudio.h"
// #include "argparse.h"
// #include "error.h"
// #include <exception>
// #include "global_config.h"
// #include "oscillators.h"
// #include "bus.h"
// #include "controller.h"
// #include "instrument.h"
// #include "midi.h"
// #include "loader/wave.h"





// #ifdef _WIN32
// #include <windows.h>
// #include <WinUser.h>
// #include <imgui.h>
// #include <imgui_impl_win32.h>
// #include <imgui_impl_dx12.h>
// #include <d3d12.h>
// #include <dxgi1_4.h>

// #include <tchar.h>
// #include <imgui_internal.h>


// #if PA_USE_ASIO
// #include "pa_asio.h"
// #endif
// #endif

// /*******************************************************************/
// static void PrintSupportedStandardSampleRates(
//         const PaStreamParameters *inputParameters,
//         const PaStreamParameters *outputParameters )
// {
//     static double standardSampleRates[] = {
//         8000.0, 9600.0, 11025.0, 12000.0, 16000.0, 22050.0, 24000.0, 32000.0,
//         44100.0, 48000.0, 88200.0, 96000.0, 192000.0, -1 /* negative terminated  list */
//     };
//     int     i, printCount;
//     PaError err;

//     printCount = 0;
//     for( i=0; standardSampleRates[i] > 0; i++ )
//     {
//         err = Pa_IsFormatSupported( inputParameters, outputParameters, standardSampleRates[i] );
//         if( err == paFormatIsSupported )
//         {
//             if( printCount == 0 )
//             {
//                 printf( "\t%8.2f", standardSampleRates[i] );
//                 printCount = 1;
//             }
//             else if( printCount == 4 )
//             {
//                 printf( ",\n\t%8.2f", standardSampleRates[i] );
//                 printCount = 1;
//             }
//             else
//             {
//                 printf( ", %8.2f", standardSampleRates[i] );
//                 ++printCount;
//             }
//         }
//     }
//     if( !printCount )
//         printf( "None\n" );
//     else
//         printf( "\n" );
// }

// int print_audio_devices()
// {
//     int     i, numDevices, defaultDisplayed;
//     const   PaDeviceInfo *deviceInfo;
//     PaStreamParameters inputParameters, outputParameters;
//     PaError err;


//     err = Pa_Initialize();
//     if( err != paNoError )
//     {
//         printf( "ERROR: Pa_Initialize returned 0x%x\n", err );
//         goto error;
//     }

//     printf( "PortAudio version: 0x%08X\n", Pa_GetVersion());
//     //printf( "Version text: '%s'\n", Pa_GetVersionInfo()->versionText );

//     numDevices = Pa_GetDeviceCount();
//     if( numDevices < 0 )
//     {
//         printf( "ERROR: Pa_GetDeviceCount returned 0x%x\n", numDevices );
//         err = numDevices;
//         goto error;
//     }

//     printf( "Number of devices = %d\n", numDevices );
//     for( i=0; i<numDevices; i++ )
//     {
//         deviceInfo = Pa_GetDeviceInfo( i );
//         printf( "--------------------------------------- device #%d\n", i );

//     /* Mark global and API specific default devices */
//         defaultDisplayed = 0;
//         if( i == Pa_GetDefaultInputDevice() )
//         {
//             printf( "[ Default Input" );
//             defaultDisplayed = 1;
//         }
//         else if( i == Pa_GetHostApiInfo( deviceInfo->hostApi )->defaultInputDevice )
//         {
//             const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
//             printf( "[ Default %s Input", hostInfo->name );
//             defaultDisplayed = 1;
//         }

//         if( i == Pa_GetDefaultOutputDevice() )
//         {
//             printf( (defaultDisplayed ? "," : "[") );
//             printf( " Default Output" );
//             defaultDisplayed = 1;
//         }
//         else if( i == Pa_GetHostApiInfo( deviceInfo->hostApi )->defaultOutputDevice )
//         {
//             const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
//             printf( (defaultDisplayed ? "," : "[") );
//             printf( " Default %s Output", hostInfo->name );
//             defaultDisplayed = 1;
//         }

//         if( defaultDisplayed )
//             printf( " ]\n" );

//     /* print device info fields */
// #ifdef WIN32
//         {   /* Use wide char on windows, so we can show UTF-8 encoded device names */
//             wchar_t wideName[MAX_PATH];
//             MultiByteToWideChar(CP_UTF8, 0, deviceInfo->name, -1, wideName, MAX_PATH-1);
//             wprintf( L"Name                        = %s\n", wideName );
//         }
// #else
//         printf( "Name                        = %s\n", deviceInfo->name );
// #endif
//         printf( "Host API                    = %s\n",  Pa_GetHostApiInfo( deviceInfo->hostApi )->name );
//         printf( "Max inputs = %d", deviceInfo->maxInputChannels  );
//         printf( ", Max outputs = %d\n", deviceInfo->maxOutputChannels  );

//         printf( "Default low input latency   = %8.4f\n", deviceInfo->defaultLowInputLatency  );
//         printf( "Default low output latency  = %8.4f\n", deviceInfo->defaultLowOutputLatency  );
//         printf( "Default high input latency  = %8.4f\n", deviceInfo->defaultHighInputLatency  );
//         printf( "Default high output latency = %8.4f\n", deviceInfo->defaultHighOutputLatency  );

// #ifdef WIN32
// #if PA_USE_ASIO
// /* ASIO specific latency information */
//         if( Pa_GetHostApiInfo( deviceInfo->hostApi )->type == paASIO ){
//             long minLatency, maxLatency, preferredLatency, granularity;

//             err = PaAsio_GetAvailableLatencyValues( i,
//                     &minLatency, &maxLatency, &preferredLatency, &granularity );

//             printf( "ASIO minimum buffer size    = %ld\n", minLatency  );
//             printf( "ASIO maximum buffer size    = %ld\n", maxLatency  );
//             printf( "ASIO preferred buffer size  = %ld\n", preferredLatency  );

//             if( granularity == -1 )
//                 printf( "ASIO buffer granularity     = power of 2\n" );
//             else
//                 printf( "ASIO buffer granularity     = %ld\n", granularity  );
//         }
// #endif /* PA_USE_ASIO */
// #endif /* WIN32 */

//         printf( "Default sample rate         = %8.2f\n", deviceInfo->defaultSampleRate );

//     /* poll for standard sample rates */
//         inputParameters.device = i;
//         inputParameters.channelCount = deviceInfo->maxInputChannels;
//         inputParameters.sampleFormat = paInt16;
//         inputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
//         inputParameters.hostApiSpecificStreamInfo = NULL;

//         outputParameters.device = i;
//         outputParameters.channelCount = deviceInfo->maxOutputChannels;
//         outputParameters.sampleFormat = paInt16;
//         outputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
//         outputParameters.hostApiSpecificStreamInfo = NULL;

//         if( inputParameters.channelCount > 0 )
//         {
//             printf("Supported standard sample rates\n for half-duplex 16 bit %d channel input = \n",
//                     inputParameters.channelCount );
//             PrintSupportedStandardSampleRates( &inputParameters, NULL );
//         }

//         if( outputParameters.channelCount > 0 )
//         {
//             printf("Supported standard sample rates\n for half-duplex 16 bit %d channel output = \n",
//                     outputParameters.channelCount );
//             PrintSupportedStandardSampleRates( NULL, &outputParameters );
//         }

//         if( inputParameters.channelCount > 0 && outputParameters.channelCount > 0 )
//         {
//             printf("Supported standard sample rates\n for full-duplex 16 bit %d channel input, %d channel output = \n",
//                     inputParameters.channelCount, outputParameters.channelCount );
//             PrintSupportedStandardSampleRates( &inputParameters, &outputParameters );
//         }
//     }

//     Pa_Terminate();

//     printf("----------------------------------------------\n");
//     return 0;

// error:
//     Pa_Terminate();
//     fprintf( stderr, "Error number: %d\n", err );
//     fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
//     return err;
// }

// /*******************************************************************/
// // Forward declare message handler from imgui_impl_win32.cpp
// extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
// {
//     if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
//         return true;

//     switch (msg)
//     {
//     case WM_SIZE:
//         return 0;
//     case WM_SYSCOMMAND:
//             return 0;
//         break;
//     case WM_DESTROY:
//         return 0;
//     }
//     return 0;
// }

// int main(int argc, char** argv);
// int main(int argc, char** argv)
// {


//     std::shared_ptr<AudioTrack> audio_track = WaveFileLoader::load("C:\\Users\\Adam\\Music\\Crazy_Dave_Intro_Theme_128.wav");

//     static int const                    NUM_FRAMES_IN_FLIGHT = 3;

    

//     // Setup Dear ImGui context
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO();
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//     io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

//     WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    
//     HWND hwnd = CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX12 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);
//     ID3D12Device*                g_pd3dDevice = nullptr;
//     ID3D12DescriptorHeap*        g_pd3dSrvDescHeap = nullptr;


//     // Setup Platform/Renderer backends
//     ImGui_ImplWin32_Init(hwnd);
//     ImGui_ImplDX12_Init(g_pd3dDevice, NUM_FRAMES_IN_FLIGHT,
//         DXGI_FORMAT_R8G8B8A8_UNORM, g_pd3dSrvDescHeap,
//         g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
//         g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart());


//     ImGui_ImplDX12_NewFrame();
//     ImGui_ImplWin32_NewFrame();
//     ImGui::NewFrame();
//     ImGui::ShowDemoWindow(); // Show demo window! :)

//     ImGui::Render();
//     ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), nullptr);

//     printf("sleeping");
//     Sleep(10000);

//     ImGui_ImplDX12_Shutdown();
//     ImGui_ImplWin32_Shutdown();
//     ImGui::DestroyContext();

//     exit(0);


//     // set up arguements
//     ArgParser argparser = ArgParser();
//     argparser.add_arguement("-d", "--devices", 0, "List all audio devices availble");
//     argparser.add_arguement("-s", "--sample-rate", 1, "sets the global sample rate", ArgTypes::ARG_INT);
//     argparser.add_arguement("-c", "--channels", 1, "sets the number of channels", ArgTypes::ARG_INT);
//     argparser.add_arguement("-f", "--frames", 1, "sets the number of frames per window", ArgTypes::ARG_INT);
//     argparser.add_arguement("-g", "--gain", 1, "adjusts master output gain. 0.0 <= gain <= 0.05. Gain will be truncated if limits exceeded", ArgTypes::ARG_FLOAT);
//     argparser.add_arguement("-cpp", "--controller-poll-period", 1, "In milliseconds, how offten to poll for controller input", ArgTypes::ARG_INT);
//     argparser.add_arguement("-o", "--master-out", 1, "Specifies output device for master bus");

//     argparser.parse(argc, argv);
//     std::cout << "Show devices = " << argparser.get_arguement("-d")->is_present() << std::endl;
//     if (argparser.get_arguement("-d")->is_present())
//     {
//         print_audio_devices();
//         exit(0);
//     }

//     if (argparser.get_arguement("-s")->is_present()) {
//         GConfig::get_instance().set_sample_rate(argparser.get_arguement("-s")->get_arg_int());
//     }

//     if (argparser.get_arguement("-c")->is_present()) {
//         GConfig::get_instance().set_num_channels(argparser.get_arguement("-c")->get_arg_int());
//     }

//     if (argparser.get_arguement("-f")->is_present()) {
//         GConfig::get_instance().set_frames_per_buffer(argparser.get_arguement("-f")->get_arg_int());
//     }

  
//     GConfig::get_instance().print_config();
//     GConfig::get_instance().check_config();


//     PaError err = Pa_Initialize();
//     if (err != paNoError)
//     {
//         critical_error_no_line_print("Failed to initialize portaudio");
//     }

//     MasterBus master_bus = MasterBus().set_gain(0.01f + argparser.get_arguement("-g")->get_arg_float());
    
//     master_bus.audio_track = audio_track;
//     master_bus.init_stream();
//     master_bus.start_stream();

// #ifdef _WIN32

//     KeyboardController kb_controller = KeyboardController();
//     kb_controller.add_key_bind(VK_MAP::X, std::bind(&KeyboardController::octave_shift_up, &kb_controller));
//     kb_controller.add_key_bind(VK_MAP::Z, std::bind(&KeyboardController::octave_shift_down, &kb_controller));
//     GControllers::get_instance().register_controller(kb_controller);
    
//     GMidi::get_instance().activate_instrument(&master_bus.synth, "sine_synth");

//     unsigned poll_period_ms = 10;
//     if (argparser.get_arguement("-cpp")->is_present()) {
//         poll_period_ms = argparser.get_arguement("-cpp")->get_arg_int();
//     }

// #endif

//     for (;;)
//     {


//     // TODO: Make a MIDI File Reader

// #ifdef _WIN32

//     // TODO: Find a way to have this be delagte instead of polling. 
//     // Update all controllers every poll_period ms
//     kb_controller.tick();
//     Sleep(poll_period_ms);

// #endif


//     }

//     return 0;
// }



















// Dear ImGui: standalone example application for DirectX 12

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// Important: to compile on 32-bit systems, the DirectX12 backend requires code to be compiled with '#define ImTextureID ImU64'.
// This is because we need ImTextureID to carry a 64-bit value and by default ImTextureID is defined as void*.
// This define is set in the example .vcxproj file and need to be replicated in your app or by adding it to your imconfig.h file.

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <tchar.h>

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

#include "imgui_internal.h"

struct FrameContext
{
    ID3D12CommandAllocator* CommandAllocator;
    UINT64                  FenceValue;
};

// Data
static int const                    NUM_FRAMES_IN_FLIGHT = 3;
static FrameContext                 g_frameContext[NUM_FRAMES_IN_FLIGHT] = {};
static UINT                         g_frameIndex = 0;

static int const                    NUM_BACK_BUFFERS = 3;
static ID3D12Device*                g_pd3dDevice = nullptr;
static ID3D12DescriptorHeap*        g_pd3dRtvDescHeap = nullptr;
static ID3D12DescriptorHeap*        g_pd3dSrvDescHeap = nullptr;
static ID3D12CommandQueue*          g_pd3dCommandQueue = nullptr;
static ID3D12GraphicsCommandList*   g_pd3dCommandList = nullptr;
static ID3D12Fence*                 g_fence = nullptr;
static HANDLE                       g_fenceEvent = nullptr;
static UINT64                       g_fenceLastSignaledValue = 0;
static IDXGISwapChain3*             g_pSwapChain = nullptr;
static bool                         g_SwapChainOccluded = false;
static HANDLE                       g_hSwapChainWaitableObject = nullptr;
static ID3D12Resource*              g_mainRenderTargetResource[NUM_BACK_BUFFERS] = {};
static D3D12_CPU_DESCRIPTOR_HANDLE  g_mainRenderTargetDescriptor[NUM_BACK_BUFFERS] = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
void WaitForLastSubmittedFrame();
FrameContext* WaitForNextFrameResources();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX12 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_HIDE);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.0f);
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX12_Init(g_pd3dDevice, NUM_FRAMES_IN_FLIGHT,
        DXGI_FORMAT_R8G8B8A8_UNORM, g_pd3dSrvDescHeap,
        g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
        g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart());

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    bool show_hello_world = true;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                done = true;
                printf("Kill given\n");
            }
        }
        if (done || !show_hello_world)
            break;

        // Handle window screen locked
        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        // Start the Dear ImGui frame
        ImGui_ImplDX12_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        if (show_hello_world) {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!", &show_hello_world);                          // Create a window called "Hello, world!" and append into it.
            printf("done = %d\n", show_hello_world);

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", nullptr);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();

        FrameContext* frameCtx = WaitForNextFrameResources();
        UINT backBufferIdx = g_pSwapChain->GetCurrentBackBufferIndex();
        frameCtx->CommandAllocator->Reset();

        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource   = g_mainRenderTargetResource[backBufferIdx];
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
        g_pd3dCommandList->Reset(frameCtx->CommandAllocator, nullptr);
        g_pd3dCommandList->ResourceBarrier(1, &barrier);

        // Render Dear ImGui graphics
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dCommandList->ClearRenderTargetView(g_mainRenderTargetDescriptor[backBufferIdx], clear_color_with_alpha, 0, nullptr);
        g_pd3dCommandList->OMSetRenderTargets(1, &g_mainRenderTargetDescriptor[backBufferIdx], FALSE, nullptr);
        g_pd3dCommandList->SetDescriptorHeaps(1, &g_pd3dSrvDescHeap);
        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_pd3dCommandList);
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT;
        g_pd3dCommandList->ResourceBarrier(1, &barrier);
        g_pd3dCommandList->Close();

        g_pd3dCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&g_pd3dCommandList);

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault(nullptr, (void*)g_pd3dCommandList);
        }

        // Present
        HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
        //HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);

        UINT64 fenceValue = g_fenceLastSignaledValue + 1;
        g_pd3dCommandQueue->Signal(g_fence, fenceValue);
        g_fenceLastSignaledValue = fenceValue;
        frameCtx->FenceValue = fenceValue;
    }

    WaitForLastSubmittedFrame();

    // Cleanup
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions
bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC1 sd;
    {
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = NUM_BACK_BUFFERS;
        sd.Width = 0;
        sd.Height = 0;
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        sd.Scaling = DXGI_SCALING_STRETCH;
        sd.Stereo = FALSE;
    }

    // [DEBUG] Enable debug interface
#ifdef DX12_ENABLE_DEBUG_LAYER
    ID3D12Debug* pdx12Debug = nullptr;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pdx12Debug))))
        pdx12Debug->EnableDebugLayer();
#endif

    // Create device
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    if (D3D12CreateDevice(nullptr, featureLevel, IID_PPV_ARGS(&g_pd3dDevice)) != S_OK)
        return false;

    // [DEBUG] Setup debug interface to break on any warnings/errors
#ifdef DX12_ENABLE_DEBUG_LAYER
    if (pdx12Debug != nullptr)
    {
        ID3D12InfoQueue* pInfoQueue = nullptr;
        g_pd3dDevice->QueryInterface(IID_PPV_ARGS(&pInfoQueue));
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
        pInfoQueue->Release();
        pdx12Debug->Release();
    }
#endif

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.NumDescriptors = NUM_BACK_BUFFERS;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        desc.NodeMask = 1;
        if (g_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dRtvDescHeap)) != S_OK)
            return false;

        SIZE_T rtvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
        for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
        {
            g_mainRenderTargetDescriptor[i] = rtvHandle;
            rtvHandle.ptr += rtvDescriptorSize;
        }
    }

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = 1;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        if (g_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dSrvDescHeap)) != S_OK)
            return false;
    }

    {
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 1;
        if (g_pd3dDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&g_pd3dCommandQueue)) != S_OK)
            return false;
    }

    for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
        if (g_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_frameContext[i].CommandAllocator)) != S_OK)
            return false;

    if (g_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_frameContext[0].CommandAllocator, nullptr, IID_PPV_ARGS(&g_pd3dCommandList)) != S_OK ||
        g_pd3dCommandList->Close() != S_OK)
        return false;

    if (g_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence)) != S_OK)
        return false;

    g_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (g_fenceEvent == nullptr)
        return false;

    {
        IDXGIFactory4* dxgiFactory = nullptr;
        IDXGISwapChain1* swapChain1 = nullptr;
        if (CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)) != S_OK)
            return false;
        if (dxgiFactory->CreateSwapChainForHwnd(g_pd3dCommandQueue, hWnd, &sd, nullptr, nullptr, &swapChain1) != S_OK)
            return false;
        if (swapChain1->QueryInterface(IID_PPV_ARGS(&g_pSwapChain)) != S_OK)
            return false;
        swapChain1->Release();
        dxgiFactory->Release();
        g_pSwapChain->SetMaximumFrameLatency(NUM_BACK_BUFFERS);
        g_hSwapChainWaitableObject = g_pSwapChain->GetFrameLatencyWaitableObject();
    }

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->SetFullscreenState(false, nullptr); g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_hSwapChainWaitableObject != nullptr) { CloseHandle(g_hSwapChainWaitableObject); }
    for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
        if (g_frameContext[i].CommandAllocator) { g_frameContext[i].CommandAllocator->Release(); g_frameContext[i].CommandAllocator = nullptr; }
    if (g_pd3dCommandQueue) { g_pd3dCommandQueue->Release(); g_pd3dCommandQueue = nullptr; }
    if (g_pd3dCommandList) { g_pd3dCommandList->Release(); g_pd3dCommandList = nullptr; }
    if (g_pd3dRtvDescHeap) { g_pd3dRtvDescHeap->Release(); g_pd3dRtvDescHeap = nullptr; }
    if (g_pd3dSrvDescHeap) { g_pd3dSrvDescHeap->Release(); g_pd3dSrvDescHeap = nullptr; }
    if (g_fence) { g_fence->Release(); g_fence = nullptr; }
    if (g_fenceEvent) { CloseHandle(g_fenceEvent); g_fenceEvent = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }

#ifdef DX12_ENABLE_DEBUG_LAYER
    IDXGIDebug1* pDebug = nullptr;
    if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug))))
    {
        pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY);
        pDebug->Release();
    }
#endif
}

void CreateRenderTarget()
{
    for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
    {
        ID3D12Resource* pBackBuffer = nullptr;
        g_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, g_mainRenderTargetDescriptor[i]);
        g_mainRenderTargetResource[i] = pBackBuffer;
    }
}

void CleanupRenderTarget()
{
    WaitForLastSubmittedFrame();

    for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
        if (g_mainRenderTargetResource[i]) { g_mainRenderTargetResource[i]->Release(); g_mainRenderTargetResource[i] = nullptr; }
}

void WaitForLastSubmittedFrame()
{
    FrameContext* frameCtx = &g_frameContext[g_frameIndex % NUM_FRAMES_IN_FLIGHT];

    UINT64 fenceValue = frameCtx->FenceValue;
    if (fenceValue == 0)
        return; // No fence was signaled

    frameCtx->FenceValue = 0;
    if (g_fence->GetCompletedValue() >= fenceValue)
        return;

    g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
    WaitForSingleObject(g_fenceEvent, INFINITE);
}

FrameContext* WaitForNextFrameResources()
{
    UINT nextFrameIndex = g_frameIndex + 1;
    g_frameIndex = nextFrameIndex;

    HANDLE waitableObjects[] = { g_hSwapChainWaitableObject, nullptr };
    DWORD numWaitableObjects = 1;

    FrameContext* frameCtx = &g_frameContext[nextFrameIndex % NUM_FRAMES_IN_FLIGHT];
    UINT64 fenceValue = frameCtx->FenceValue;
    if (fenceValue != 0) // means no fence was signaled
    {
        frameCtx->FenceValue = 0;
        g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
        waitableObjects[1] = g_fenceEvent;
        numWaitableObjects = 2;
    }

    WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);

    return frameCtx;
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            WaitForLastSubmittedFrame();
            CleanupRenderTarget();
            HRESULT result = g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT);
            assert(SUCCEEDED(result) && "Failed to resize swapchain.");
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}