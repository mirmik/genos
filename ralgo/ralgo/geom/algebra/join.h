#ifndef RALGO_GEOMALG_JOIN_H
#define RALGO_GEOMALG_JOIN_H

#include <ralgo/geom/algebra/bivector4d.h>
#include <ralgo/geom/algebra/join.h>
#include <ralgo/geom/algebra/product.h>
#include <ralgo/geom/algebra/trivector4d.h>

namespace ralgo
{
    namespace geomalg
    {

        template <class T>
        ralgo::geomalg::bivector4d<T>
        line_containing_points(const ralgo::geomalg::point3d<T> &p,
                               const ralgo::geomalg::point3d<T> &q)
        {
            return wedge4d(p, q);
        }

        template <class T>
        ralgo::geomalg::bivector4d<T>
        line_containing_points(const ralgo::geomalg::vector4d<T> &p,
                               const ralgo::geomalg::vector4d<T> &q)
        {
            return wedge4d(p, q);
        }

        template <class T>
        ralgo::geomalg::trivector4d<T>
        plane_containing_line_and_point(const ralgo::geomalg::bivector4d<T> &l,
                                        const ralgo::geomalg::vector4d<T> &p)
        {
            return wedge4d(l, p);
        }

        template <class T>
        ralgo::geomalg::trivector4d<T>
        plane_containing_points(const ralgo::geomalg::point<T> &a,
                                const ralgo::geomalg::point<T> &b,
                                const ralgo::geomalg::point<T> &c)
        {
            return wedge4d(wedge4d(a, b), c);
        }

        template <class T>
        ralgo::geomalg::trivector4d<T>
        plane_containing_points(const ralgo::geomalg::point3d<T> &a,
                                const ralgo::geomalg::point3d<T> &b,
                                const ralgo::geomalg::point3d<T> &c)
        {
            return wedge4d(wedge4d(a, b), c);
        }

        template <class T>
        ralgo::geomalg::bivector4d<T> join(const ralgo::geomalg::point<T> &a,
                                           const ralgo::geomalg::point<T> &b)
        {
            return line_containing_points(a, b);
        }

        template <class T>
        ralgo::geomalg::bivector4d<T> join(const ralgo::geomalg::point3d<T> &a,
                                           const ralgo::geomalg::point3d<T> &b)
        {
            return line_containing_points(a, b);
        }

        template <class T>
        ralgo::geomalg::trivector4d<T> join(const ralgo::geomalg::point4d<T> &a,
                                            const ralgo::geomalg::point4d<T> &b,
                                            const ralgo::geomalg::point4d<T> &c)
        {
            return plane_containing_points(a, b, c);
        }

        template <class T>
        ralgo::geomalg::trivector4d<T> join(const ralgo::geomalg::point3d<T> &a,
                                            const ralgo::geomalg::point3d<T> &b,
                                            const ralgo::geomalg::point3d<T> &c)
        {
            return plane_containing_points(a, b, c);
        }

        template <class T>
        ralgo::geomalg::trivector4d<T>
        join(const ralgo::geomalg::bivector4d<T> &l,
             const ralgo::geomalg::vector4d<T> &p)
        {
            return plane_containing_line_and_point(l, p);
        }
    }
}

#endif