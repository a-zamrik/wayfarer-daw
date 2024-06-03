#ifndef BUS_H
#define BUS_H

#include "generators.h"
#include "../include/portaudio.h"
  
class MasterBus
{
private:

    // Place holder variables
    Frame frame;
    Sine  sin;

    // TODO: Instrument / Generator / Audio Input

    // TODO: Audio Effects
    // std::vector<AudioEffect> effect_chain;

    float gain;

    PaStream *stream;
    static int paCallback( const void *inputBuffer, void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData );

public:
    MasterBus() : frame(), sin(400), gain(0.01f) {}

    inline MasterBus& set_gain(float _gain) {this->gain = _gain; return *this;}

    void init_stream();

    void start_stream();

    void stop_stream();

};

#endif