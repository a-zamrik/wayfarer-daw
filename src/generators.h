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