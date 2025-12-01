#ifndef IGRIS_MATH_INVSQRT_H
#define IGRIS_MATH_INVSQRT_H

#include <igris/compiler.h>
#include <cstdint>

union float_int32_u
{
    float y;
    int32_t i;
};

__BEGIN_DECLS

static inline float veryquick_invsqrt(float number)
{
    float x2;
    union float_int32_u u;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    u.y = number;
    // i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
    u.i = 0x5F375A86 - (u.i >> 1);               // what the fuck?
    u.y = u.y * (threehalfs - (x2 * u.y * u.y)); // 1st iteration
    // u.y  = u.y * ( threehalfs - ( x2 * u.y * u.y ) );   // 2nd iteration,
    // this can be removed

    return u.y;
}

static inline float quick_invsqrt(float number)
{
    float x2; //, y;
    union float_int32_u u;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    u.y = number;
    // i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
    u.i = 0x5F375A86 - (u.i >> 1);               // what the fuck?
    u.y = u.y * (threehalfs - (x2 * u.y * u.y)); // 1st iteration
    u.y = u.y *
          (threehalfs - (x2 * u.y * u.y)); // 2nd iteration, this can be removed

    return u.y;
}

__END_DECLS

#endif
