#include "bus.h"
#include "error.h"
#include "global_config.h"
#include "controller.h"
#include <iostream>

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

    // Sin generotor populate frame
    MasterBus->sin.filter(MasterBus->frame);

    // Pass frame info to output 
    for (unsigned i = 0; i < framesPerBuffer; i++)
    {
        for (unsigned c = 0; c < n_channels; c++)
        {
            *out++ = MasterBus->gain * MasterBus->frame(c,i); 
        }
    }

    // Update all controllers, need to sync with frame updates
    GControllers::get_instance().tick(framesPerBuffer);

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
