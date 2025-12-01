#ifndef IGRIS_MATH_RANDOM_H
#define IGRIS_MATH_RANDOM_H

//Утилиты для использования рандомайзера.

inline float float_random(float a, float b)
{
    return a + (rand() * (b - a) / RAND_MAX);
}

namespace igris
{
    template <typename T>
    inline T random_minmax(const T &minval, const T &maxval);

    template <>
    inline float random_minmax<float>(const float &a, const float &b)
    {
        return a + (rand() * (b - a) / RAND_MAX);
    }

    template <>
    inline double random_minmax<double>(const double &a, const double &b)
    {
        return a + (rand() * (b - a) / RAND_MAX);
    }
}

#endif
