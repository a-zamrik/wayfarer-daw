/** @file pa_devs.c
    @ingroup examples_src
    @brief List available devices, including device information.
    @author Phil Burk http://www.softsynth.com

    @note Define PA_USE_ASIO=0 to compile this code on Windows without
        ASIO support.
*/
/*
 * $Id$
 *
 * This program uses the PortAudio Portable Audio Library.
 * For more information see: http://www.portaudio.com
 * Copyright (c) 1999-2000 Ross Bencina and Phil Burk
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The text above constitutes the entire PortAudio license; however,
 * the PortAudio community also makes the following non-binding requests:
 *
 * Any person wishing to distribute modifications to the Software is
 * requested to send the modifications to the original developer so that
 * they can be incorporated into the canonical version. It is also
 * requested that these non-binding requests be included along with the
 * license above.
 */

#include <stdio.h>
#include <iostream>
#include <math.h>
#include "../include/portaudio.h"
#include "argparse.h"
#include "error.h"
#include <exception>
#include "global_config.h"
#include "oscillators.h"
#include "bus.h"
#include "controller.h"
#include "instrument.h"
#include "midi.h"
#include "loader/wave.h"


#ifdef _WIN32
#include <windows.h>
#include <WinUser.h>

#if PA_USE_ASIO
#include "pa_asio.h"
#endif
#endif

/*******************************************************************/
static void PrintSupportedStandardSampleRates(
        const PaStreamParameters *inputParameters,
        const PaStreamParameters *outputParameters )
{
    static double standardSampleRates[] = {
        8000.0, 9600.0, 11025.0, 12000.0, 16000.0, 22050.0, 24000.0, 32000.0,
        44100.0, 48000.0, 88200.0, 96000.0, 192000.0, -1 /* negative terminated  list */
    };
    int     i, printCount;
    PaError err;

    printCount = 0;
    for( i=0; standardSampleRates[i] > 0; i++ )
    {
        err = Pa_IsFormatSupported( inputParameters, outputParameters, standardSampleRates[i] );
        if( err == paFormatIsSupported )
        {
            if( printCount == 0 )
            {
                printf( "\t%8.2f", standardSampleRates[i] );
                printCount = 1;
            }
            else if( printCount == 4 )
            {
                printf( ",\n\t%8.2f", standardSampleRates[i] );
                printCount = 1;
            }
            else
            {
                printf( ", %8.2f", standardSampleRates[i] );
                ++printCount;
            }
        }
    }
    if( !printCount )
        printf( "None\n" );
    else
        printf( "\n" );
}

int print_audio_devices()
{
    int     i, numDevices, defaultDisplayed;
    const   PaDeviceInfo *deviceInfo;
    PaStreamParameters inputParameters, outputParameters;
    PaError err;


    err = Pa_Initialize();
    if( err != paNoError )
    {
        printf( "ERROR: Pa_Initialize returned 0x%x\n", err );
        goto error;
    }

    printf( "PortAudio version: 0x%08X\n", Pa_GetVersion());
    //printf( "Version text: '%s'\n", Pa_GetVersionInfo()->versionText );

    numDevices = Pa_GetDeviceCount();
    if( numDevices < 0 )
    {
        printf( "ERROR: Pa_GetDeviceCount returned 0x%x\n", numDevices );
        err = numDevices;
        goto error;
    }

    printf( "Number of devices = %d\n", numDevices );
    for( i=0; i<numDevices; i++ )
    {
        deviceInfo = Pa_GetDeviceInfo( i );
        printf( "--------------------------------------- device #%d\n", i );

    /* Mark global and API specific default devices */
        defaultDisplayed = 0;
        if( i == Pa_GetDefaultInputDevice() )
        {
            printf( "[ Default Input" );
            defaultDisplayed = 1;
        }
        else if( i == Pa_GetHostApiInfo( deviceInfo->hostApi )->defaultInputDevice )
        {
            const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
            printf( "[ Default %s Input", hostInfo->name );
            defaultDisplayed = 1;
        }

        if( i == Pa_GetDefaultOutputDevice() )
        {
            printf( (defaultDisplayed ? "," : "[") );
            printf( " Default Output" );
            defaultDisplayed = 1;
        }
        else if( i == Pa_GetHostApiInfo( deviceInfo->hostApi )->defaultOutputDevice )
        {
            const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
            printf( (defaultDisplayed ? "," : "[") );
            printf( " Default %s Output", hostInfo->name );
            defaultDisplayed = 1;
        }

        if( defaultDisplayed )
            printf( " ]\n" );

    /* print device info fields */
#ifdef WIN32
        {   /* Use wide char on windows, so we can show UTF-8 encoded device names */
            wchar_t wideName[MAX_PATH];
            MultiByteToWideChar(CP_UTF8, 0, deviceInfo->name, -1, wideName, MAX_PATH-1);
            wprintf( L"Name                        = %s\n", wideName );
        }
#else
        printf( "Name                        = %s\n", deviceInfo->name );
#endif
        printf( "Host API                    = %s\n",  Pa_GetHostApiInfo( deviceInfo->hostApi )->name );
        printf( "Max inputs = %d", deviceInfo->maxInputChannels  );
        printf( ", Max outputs = %d\n", deviceInfo->maxOutputChannels  );

        printf( "Default low input latency   = %8.4f\n", deviceInfo->defaultLowInputLatency  );
        printf( "Default low output latency  = %8.4f\n", deviceInfo->defaultLowOutputLatency  );
        printf( "Default high input latency  = %8.4f\n", deviceInfo->defaultHighInputLatency  );
        printf( "Default high output latency = %8.4f\n", deviceInfo->defaultHighOutputLatency  );

#ifdef WIN32
#if PA_USE_ASIO
/* ASIO specific latency information */
        if( Pa_GetHostApiInfo( deviceInfo->hostApi )->type == paASIO ){
            long minLatency, maxLatency, preferredLatency, granularity;

            err = PaAsio_GetAvailableLatencyValues( i,
                    &minLatency, &maxLatency, &preferredLatency, &granularity );

            printf( "ASIO minimum buffer size    = %ld\n", minLatency  );
            printf( "ASIO maximum buffer size    = %ld\n", maxLatency  );
            printf( "ASIO preferred buffer size  = %ld\n", preferredLatency  );

            if( granularity == -1 )
                printf( "ASIO buffer granularity     = power of 2\n" );
            else
                printf( "ASIO buffer granularity     = %ld\n", granularity  );
        }
#endif /* PA_USE_ASIO */
#endif /* WIN32 */

        printf( "Default sample rate         = %8.2f\n", deviceInfo->defaultSampleRate );

    /* poll for standard sample rates */
        inputParameters.device = i;
        inputParameters.channelCount = deviceInfo->maxInputChannels;
        inputParameters.sampleFormat = paInt16;
        inputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
        inputParameters.hostApiSpecificStreamInfo = NULL;

        outputParameters.device = i;
        outputParameters.channelCount = deviceInfo->maxOutputChannels;
        outputParameters.sampleFormat = paInt16;
        outputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
        outputParameters.hostApiSpecificStreamInfo = NULL;

        if( inputParameters.channelCount > 0 )
        {
            printf("Supported standard sample rates\n for half-duplex 16 bit %d channel input = \n",
                    inputParameters.channelCount );
            PrintSupportedStandardSampleRates( &inputParameters, NULL );
        }

        if( outputParameters.channelCount > 0 )
        {
            printf("Supported standard sample rates\n for half-duplex 16 bit %d channel output = \n",
                    outputParameters.channelCount );
            PrintSupportedStandardSampleRates( NULL, &outputParameters );
        }

        if( inputParameters.channelCount > 0 && outputParameters.channelCount > 0 )
        {
            printf("Supported standard sample rates\n for full-duplex 16 bit %d channel input, %d channel output = \n",
                    inputParameters.channelCount, outputParameters.channelCount );
            PrintSupportedStandardSampleRates( &inputParameters, &outputParameters );
        }
    }

    Pa_Terminate();

    printf("----------------------------------------------\n");
    return 0;

error:
    Pa_Terminate();
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}

/*******************************************************************/


int main(int argc, char** argv);
int main(int argc, char** argv)
{
    WaveFileLoader wave_loader = WaveFileLoader("C:\\Users\\Adam\\Music\\The Mamas & the Papas - California Dreamin'.wav");


    exit(0);
    

    // set up arguements
    ArgParser argparser = ArgParser();
    argparser.add_arguement("-d", "--devices", 0, "List all audio devices availble");
    argparser.add_arguement("-s", "--sample-rate", 1, "sets the global sample rate", ArgTypes::ARG_INT);
    argparser.add_arguement("-c", "--channels", 1, "sets the number of channels", ArgTypes::ARG_INT);
    argparser.add_arguement("-f", "--frames", 1, "sets the number of frames per window", ArgTypes::ARG_INT);
    argparser.add_arguement("-g", "--gain", 1, "adjusts master output gain. 0.0 <= gain <= 0.05. Gain will be truncated if limits exceeded", ArgTypes::ARG_FLOAT);
    argparser.add_arguement("-cpp", "--controller-poll-period", 1, "In milliseconds, how offten to poll for controller input", ArgTypes::ARG_INT);
    argparser.add_arguement("-o", "--master-out", 1, "Specifies output device for master bus");

    argparser.parse(argc, argv);
    std::cout << "Show devices = " << argparser.get_arguement("-d")->is_present() << std::endl;
    if (argparser.get_arguement("-d")->is_present())
    {
        print_audio_devices();
        exit(0);
    }

    if (argparser.get_arguement("-s")->is_present()) {
        GConfig::get_instance().set_sample_rate(argparser.get_arguement("-s")->get_arg_int());
    }

    if (argparser.get_arguement("-c")->is_present()) {
        GConfig::get_instance().set_num_channels(argparser.get_arguement("-c")->get_arg_int());
    }

    if (argparser.get_arguement("-f")->is_present()) {
        GConfig::get_instance().set_frames_per_buffer(argparser.get_arguement("-f")->get_arg_int());
    }

  
    GConfig::get_instance().print_config();
    GConfig::get_instance().check_config();


    PaError err = Pa_Initialize();
    if (err != paNoError)
    {
        critical_error_no_line_print("Failed to initialize portaudio");
    }

    MasterBus master_bus = MasterBus().set_gain(0.01f + argparser.get_arguement("-g")->get_arg_float());
    master_bus.init_stream();
    master_bus.start_stream();

#ifdef _WIN32

    KeyboardController kb_controller = KeyboardController();
    kb_controller.add_key_bind(VK_MAP::X, std::bind(&KeyboardController::octave_shift_up, &kb_controller));
    kb_controller.add_key_bind(VK_MAP::Z, std::bind(&KeyboardController::octave_shift_down, &kb_controller));
    GControllers::get_instance().register_controller(kb_controller);
    
    GMidi::get_instance().activate_instrument(&master_bus.synth, "sine_synth");

    unsigned poll_period_ms = 10;
    if (argparser.get_arguement("-cpp")->is_present()) {
        poll_period_ms = argparser.get_arguement("-cpp")->get_arg_int();
    }

#endif

    for (;;)
    {


    // TODO: Make a MIDI File Reader

#ifdef _WIN32

    // TODO: Find a way to have this be delagte instead of polling. 
    // Update all controllers every poll_period ms
    kb_controller.tick();
    Sleep(poll_period_ms);

#endif


    }

    return 0;
}