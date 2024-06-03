#ifndef GENERATORS_H
#define GENERATORS_H

#include <vector>
#include <memory>

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
    std::vector<float> wave_table;
    uint32_t           wave_index = 0;

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
    int hz;

public:
    Sine() {}
    Sine(int _hz) : hz(_hz), t(0) {}

    virtual float get_next_sample();

};




// Interface class. Samples are generated and cached for later
class CachedGenerator
{

protected:
    std::vector<float> wave_table;
    uint32_t           wave_index = 0;

public:
  // Empty virtual destructor for proper cleanup
  virtual ~CachedGenerator() {}

  void filter(Frame & frame);
};


class CachedSine : public CachedGenerator
{
public:
    CachedSine(int hz);
};






#endif