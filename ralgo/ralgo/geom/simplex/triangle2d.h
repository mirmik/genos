#ifndef RALGO_GEOM_SIMPLEX_TRIANGLE_32_H
#define RALGO_GEOM_SIMPLEX_TRIANGLE_2D_H

#include <ralgo/geom/vector2d.h>
#include <ralgo/linalg/linalg.h>

namespace ralgo
{
    namespace geom
    {
        namespace simplex
        {
            template <class T> class triangle2d
            {
                point2d<T> _a;
                point2d<T> _b;
                point2d<T> _c;

            public:
                triangle2d(const point2d<T> &a,
                           const point2d<T> &b,
                           const point2d<T> &c)
                    : _a(a), _b(b), _c(c)
                {
                }

                linalg::vec<T, 3> baricentric_of_point2d(const point2d<T> &p)
                {
                    // https://habr.com/ru/post/249467/
                    auto ab = _b - _a;
                    auto ac = _c - _a;
                    auto pa = _a - p;
                    auto x = linalg::vec<T, 3>{ab.x(), ac.x(), pa.x()};
                    auto y = linalg::vec<T, 3>{ab.y(), ac.y(), pa.y()};
                    auto z = linalg::cross(x, y);
                    auto r = z / z.z;
                    return {1 - r.x - r.y, r.x, r.y};
                }
            };
        }
    }
}

#endif