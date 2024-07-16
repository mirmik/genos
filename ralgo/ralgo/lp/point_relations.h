#ifndef RALGO_POINT_RELATION_H
#define RALGO_POINT_RELATION_H

#include <ralgo/linalg/linalg.h>

template <class T>
T line_over_point2_argument(const linalg::vec<T, 2> &a,
                            const linalg::vec<T, 2> &b,
                            const linalg::vec<T, 2> &pnt)
{
    auto x = normalize(a - pnt);
    auto y = normalize(b - pnt);
    auto ret = cross(x, y) > 0 ? angle(x, y) : -angle(x, y);

    return ret;
}

template <class T>
T polyline_over_point2_argument(const linalg::vec<T, 2> *polyline,
                                int size,
                                const linalg::vec<T, 2> &pnt,
                                bool closed = false)
{
    auto first = polyline[0];

    T accumulator = 0;
    for (int i = 1; i < size; ++i)
    {
        auto second = polyline[i];
        accumulator += line_over_point2_argument(first, second, pnt);
        first = second;
    }

    if (closed)
    {
        accumulator +=
            line_over_point2_argument(polyline[size - 1], polyline[0], pnt);
    }

    return accumulator;
}

template <class T>
bool point2_in_polygon(const linalg::vec<T, 2> &pnt,
                       const linalg::vec<T, 2> *polyline,
                       int size)
{
    auto angle = polyline_over_point2_argument(polyline, size, pnt, true);

    return fabs(angle - 2 * M_PI) < 1e-5 || fabs(angle + 2 * M_PI) < 1e-5;
}

#endif
