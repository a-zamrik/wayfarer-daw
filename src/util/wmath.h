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

/* https://stackoverflow.com/questions/32320028/convert-linear-audio-frequency-distribution-to-logarithmic-perceptual-distributi
    By Jaket
    start - start frequency
    stop - stop frequency
    n - the point which you wish to compute (zero based)
    N - the number of points over which to divide the frequency range

    logspace(20, 200000, 0, 4) = 20
    logspace(20, 200000, 1, 4) = 200
    logspace(20, 200000, 2, 4) = 2000
    logspace(20, 200000, 3, 4) = 20000
*/
float logspace(float start, float stop, int n, int N);


#endif