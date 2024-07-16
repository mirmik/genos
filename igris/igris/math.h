#ifndef IGRIS_MATH_H
#define IGRIS_MATH_H

#include <igris/math/convertors.h>
#include <igris/math/defs.h>

#define CLAMP(val, lo, hi) ((val) < (lo) ? (lo) : (val) > (hi) ? (hi) : (val))

#ifdef __cplusplus

namespace igris
{
    template <class T, class A, class B>
    T clamp(const T &c, const A &a, const B &b)
    {
        return c < a ? a : c > b ? b : c;
    }

    template <class T, class A, class B>
    bool inside(const T &c, const A &a, const B &b)
    {
        return c > a && c < b;
    }

    template <class T, class A, class B>
    bool outside(const T &c, const A &a, const B &b)
    {
        return c < a || c > b;
    }

    using ::deg;
    using ::deg2rad;
    using ::rad2deg;

    template <class A, class B, class C> constexpr bool early(A a, B b, C c)
    {
        return abs(a - b) < c;
    }

    template <class A, class B> constexpr bool early(A a, B b)
    {
        return early(a, b, 1e-6);
    }
}

#endif

#endif
