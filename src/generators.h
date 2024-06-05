#ifndef GENERATORS_H
#define GENERATORS_H

#include <vector>
#include <memory>
#include "envelope.h"

class Frame
{
private:
    std::vector<std::vector<float>> samples;

public:
    Frame();
    inline float& operator()(int c, int s) {return this->samples[c][s];} 

};

class Generator
{

protected:


public:
  // Empty virtual destructor for proper cleanup
  virtual ~Generator() {}

  void filter(Frame & frame);
  virtual float get_next_sample() = 0;
};

class Sine : public Generator
{

private:
    int t = 0;
    float hz = 400;
    float phase_shift = 0;

    Envelope adsr_env;

    bool on = false;

public:

    // TODO: add isDriving 
    bool driving = false;

    
    Sine(float _hz) : hz(_hz), t(0), adsr_env(0.03f, 1.0f, 0.1f, 1.0f, 0.5f) {on=false;}
    virtual float get_next_sample();

    void set_hz(float);
    void hz_increase() {this->set_hz(this->hz + 50.0f);}
    void hz_decrease() {this->set_hz(this->hz - 50.0f);}

    void turn_on() {
        if (!this->on) {
            this->on = true; 
            adsr_env.reset();
        }
    }
    void turn_off() {this->on = false;}
    bool is_on() {return this->on;}

};




// Interface class. Samples are generated and cached for later
class WaveTable
{

protected:
    // TODO: Turn into circular buffer
    std::vector<float> wave_table;
    float              wave_index = 0;
    float              wave_step = 0;
    float              min_hz;
    float              hz;

public:
  // Empty virtual destructor for proper cleanup
  virtual ~WaveTable() {}

  void filter(Frame & frame);
  inline virtual float get_next_sample();
};


class WTSine : public WaveTable
{
public:
    WTSine(float _min_hz, float _hz);


    void hz_increase() {this->wave_step += 0.001f; }
    void hz_decrease() {this->wave_step -= 0.001f; }
};






#endif