#ifndef RALGO_GEOMALG_DISTANCE_H
#define RALGO_GEOMALG_DISTANCE_H

#include <ralgo/geom/algebra/bivector4d.h>
#include <ralgo/geom/algebra/magnitude.h>
#include <ralgo/geom/algebra/vector4d.h>

namespace ralgo
{
    namespace geomalg
    {
        template <class T>
        magnitude<T> distance_between_points(const vector4d<T> &p,
                                             const vector4d<T> &q)
        {
            auto x = q.x() * p.w() - p.x() * q.w();
            auto y = q.y() * p.w() - p.y() * q.w();
            auto z = q.z() * p.w() - p.z() * q.w();
            auto s = std::sqrt(x * x + y * y + z * z);
            auto w = p.w() * q.w();
            return {s, w};
        }

        template <class T>
        magnitude<T> distance_between_point_and_line(const vector4d<T> &p,
                                                     const bivector4d<T> &l)
        {
            auto x = l.vy() * p.z() - l.vz() * p.y() + l.mx() * p.w();
            auto y = l.vz() * p.x() - l.vx() * p.z() + l.my() * p.w();
            auto z = l.vx() * p.y() - l.vy() * p.x() + l.mz() * p.w();
            auto s = std::sqrt(x * x + y * y + z * z);
            auto w = std::abs(p.w()) * l.weight().norm();
            return {s, w};
        }

    }
}

#endif