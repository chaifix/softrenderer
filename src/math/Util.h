#ifndef __UTIL_H
#define __UTIL_H
#include <cmath>

#define M_PI 3.1415926

#define clamp(x, minv, maxv) \
std::fmax(std::fmin(x, maxv), minv)

#define max std::fmax 
#define min std::fmin

// Converts degrees to radians.
//#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

inline float degreesToRadians(float angle)
{
    return angle * M_PI / 180.0;
}

// Converts radians to degrees.
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

#endif