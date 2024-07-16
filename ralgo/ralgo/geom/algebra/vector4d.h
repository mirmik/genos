#ifndef RALGO_GEOMALG_VECTOR4D_H
#define RALGO_GEOMALG_VECTOR4D_H

#include <ralgo/geom/vector3d.h>
#include <ralgo/linalg/linalg.h>

namespace ralgo
{
    namespace geomalg
    {
        using ralgo::geom::vector3d;

        template <class T> class vector4d
        {
            vector3d<T> _xyz;
            T _w;

        public:
            vector4d() = default;
            vector4d(T x, T y, T z, T w) : _xyz(x, y, z), _w(w) {}
            vector4d(const vector4d &v) = default;
            vector4d &operator=(const vector4d &v) = default;

            T x() const
            {
                return _xyz.x();
            }

            T y() const
            {
                return _xyz.y();
            }

            T z() const
            {
                return _xyz.z();
            }

            T w() const
            {
                return _w;
            }

            const vector3d<T> &bulk() const
            {
                return _xyz;
            }

            T weight() const
            {
                return _w;
            }

            vector4d unitized()
            {
                return vector4d(this->x() / this->w(),
                                this->y() / this->w(),
                                this->z() / this->w(),
                                1);
            }
        };

        template <class T> class point4d : public vector4d<T>
        {
        public:
            point4d() = default;
            point4d(T x, T y, T z, T w) : vector4d<T>(x, y, z, w) {}
            point4d(T x, T y, T z) : vector4d<T>(x, y, z, 1) {}

            point4d unitized() const
            {
                return point4d(this->x() / this->w(),
                               this->y() / this->w(),
                               this->z() / this->w(),
                               1);
            }
        };

        template <class T> using point = point4d<T>;
    }
}

#endif