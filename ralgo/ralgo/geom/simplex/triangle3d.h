#ifndef RALGO_GEOM_SIMPLEX_TRIANGLE_3D_H
#define RALGO_GEOM_SIMPLEX_TRIANGLE_3D_H

#include <ralgo/geom/algebra/join.h>
#include <ralgo/geom/algebra/meet.h>
#include <ralgo/geom/algebra/trivector4d.h>
#include <ralgo/geom/parametric/ray3d.h>
#include <ralgo/geom/to_algebraic.h>
#include <ralgo/geom/vector3d.h>

namespace ralgo
{
    namespace geom
    {
        namespace simplex
        {
            using ralgo::geomalg::trivector4d;
            using ralgo::geompar::ray3d;

            template <class T> class triangle3d
            {
                point3d<T> _a;
                point3d<T> _b;
                point3d<T> _c;

            public:
                triangle3d(const point3d<T> &a,
                           const point3d<T> &b,
                           const point3d<T> &c)
                    : _a(a), _b(b), _c(c)
                {
                }

                trivector4d<T> plane() const
                {
                    return ralgo::geomalg::join(_a, _b, _c);
                }

                bool is_intersect(const ray3d<T> &ray) const
                {
                    auto line = ralgo::geom::to_algebraic(ray);
                    auto plane = this->plane();
                    auto intersection4d =
                        ralgo::geomalg::meet(line, plane).unitized();
                    auto intersection = intersection4d.bulk();

                    auto ab = _b - _a;
                    auto bc = _c - _b;
                    auto ca = _a - _c;

                    auto ap = intersection - _a;
                    auto bp = intersection - _b;
                    auto cp = intersection - _c;

                    auto abap = cross(ab, ap);
                    auto bcbp = cross(bc, bp);
                    auto ccap = cross(ca, cp);

                    auto r1 = dot(abap, bcbp);
                    auto r2 = dot(bcbp, ccap);

                    return r1 > 0 && r2 > 0;
                }
            };
        }
    }
}

#endif