#ifndef IGRIS_MATH_DEG_H
#define IGRIS_MATH_DEG_H

#include <igris/math/defs.h>
#include <math.h>

#ifdef __cplusplus
template <class T> constexpr T deg2rad(const T &x)
{
    return x * (T)(M_PI / 180.0);
}
template <class T> constexpr T rad2deg(const T &x)
{
    return x * (T)(180.0 / M_PI);
}
template <class T> constexpr T deg(const T &x)
{
    return x * (T)(M_PI / 180.0);
}

#else

static inline double deg2rad(double x)
{
    return x * M_PI / 180.0;
}
static inline double rad2deg(double x)
{
    return x * 180.0 / M_PI;
}

static inline float deg2radf(float x)
{
    return x * (float)(M_PI / 180.0);
}

static inline float rad2degf(float x)
{
    return x * (float)(180.0 / M_PI);
}

static inline double deg(double x)
{
    return x * M_PI / 180.0;
}
static inline float degf(float x)
{
    return x * (float)(M_PI / 180.0);
}
#endif

#define DEG2RAD (M_PI / 180.0)
#define RAD2DEG (180.0 / M_PI)

#endif
