#ifndef RALGO_GEOMALG_MEET_H
#define RALGO_GEOMALG_MEET_H

#include <ralgo/geom/algebra/bivector4d.h>
#include <ralgo/geom/algebra/trivector4d.h>

namespace ralgo
{
    namespace geomalg
    {
        template <class T>
        ralgo::geomalg::bivector4d<T> line_where_plane_intersects_plane(
            const ralgo::geomalg::trivector4d<T> &g,
            const ralgo::geomalg::trivector4d<T> &h)
        {
            return antiwedge4d(g, h);
        }

        template <class T>
        ralgo::geomalg::point<T> point_where_line_intersects_plane(
            const ralgo::geomalg::bivector4d<T> &l,
            const ralgo::geomalg::trivector4d<T> &g)
        {
            return antiwedge4d(l, g);
        }

        template <class T>
        ralgo::geomalg::bivector4d<T>
        meet(const ralgo::geomalg::trivector4d<T> &g,
             const ralgo::geomalg::trivector4d<T> &h)
        {
            return line_where_plane_intersects_plane(g, h);
        }

        template <class T>
        ralgo::geomalg::point<T> meet(const ralgo::geomalg::bivector4d<T> &l,
                                      const ralgo::geomalg::trivector4d<T> &g)
        {
            return point_where_line_intersects_plane(l, g);
        }
    }
}
#endif