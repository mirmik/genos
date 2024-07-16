#ifndef RALGO_GEOMALG_TRIVECTOR4D_H
#define RALGO_GEOMALG_TRIVECTOR4D_H

#include <ralgo/linalg/linalg.h>

namespace ralgo
{
    namespace geomalg
    {
        template <class T> class trivector4d
        {
            linalg::vec<T, 3> _xyz;
            T _w;

        public:
            trivector4d() = default;
            trivector4d(T x, T y, T z, T w) : _xyz(x, y, z), _w(w) {}
            trivector4d(const linalg::vec<T, 3> &xyz, T w) : _xyz(xyz), _w(w) {}
            trivector4d(const trivector4d &v) = default;
            trivector4d &operator=(const trivector4d &v) = default;

            T x() const
            {
                return _xyz.x;
            }

            T y() const
            {
                return _xyz.y;
            }

            T z() const
            {
                return _xyz.z;
            }

            T w() const
            {
                return _w;
            }

            trivector4d unitized() const
            {
                auto n = linalg::length(_xyz);
                return trivector4d(_xyz / n, _w / n);
            }
        };
    }
}

#endif