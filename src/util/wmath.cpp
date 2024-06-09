#include<cmath>


float 
lerp(float a, float b, float t) {
  return (1 - t) * a + t * b;
}

float
linear_to_db(float float_pcm)
{
    float abs = std::abs(float_pcm);
    if (abs <= 0)
    {
        return -144.0f; // -inf db essentially
    }
    else
    {
        return 20.0f * log10f(abs);
    }
}

float
db_to_linear(float float_db)
{
    return powf(10.0f, float_db / 20.0f);
}
    

