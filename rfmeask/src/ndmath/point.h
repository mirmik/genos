#ifndef RALGO_ND_POINT_H
#define RALGO_ND_POINT_H

#include <ralgo/linalg/vector.h>
#include <ralgo/linspace.h>

namespace ndmath
{
    using point = ralgo::vector<double>;
    using vector = ralgo::vector<double>;

    static inline std::vector<ndmath::vector>
    uniform_points(ndmath::point a, ndmath::point b, int points)
    {
        if (points < 2)
            points = 2;

        std::vector<ndmath::vector> ret;
        ralgo::linspace<ndmath::vector> linspace(a, b, points);
        for (auto pnt : linspace)
        {
            ret.push_back(pnt);
        }
        return ret;
    }
}

#endif