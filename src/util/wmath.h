#ifndef _WMATH_H_
#define _WMATH_H_

#define PI_F (3.14159265359f)
#define TWO_PI_F (6.28318530718f)

/*  Linear Interpolation
    t = 0 -> returns a
    t = 1 -> returns b
*/
float lerp(float a, float b, float t);

/*  Convert linear PCM to decibel scale
*/
float linear_to_db(float float_pcm);

/*  Convert decibel to linear PCM
*/
float db_to_linear(float float_db);
    


#endif