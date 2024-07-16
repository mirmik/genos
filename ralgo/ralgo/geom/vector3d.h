#ifndef RALGO_GEOMALG_VECTOR3D_H
#define RALGO_GEOMALG_VECTOR3D_H

#include <ralgo/linalg/linalg.h>

namespace ralgo
{
    namespace geom
    {
        template <class T> class vector3d
        {
            linalg::vec<T, 3> _data;

        public:
            vector3d() = default;
            vector3d(T x, T y, T z) : _data(x, y, z) {}
            vector3d(const linalg::vec<T, 3> &v) : _data(v) {}
            vector3d(const vector3d &v) = default;
            vector3d &operator=(const vector3d &v) = default;

            T x() const
            {
                return _data[0];
            }

            T y() const
            {
                return _data[1];
            }

            T z() const
            {
                return _data[2];
            }

            const linalg::vec<T, 3> &xyz() const
            {
                return _data;
            }

            T norm() const
            {
                return linalg::length(_data);
            }

            T norm2() const
            {
                return linalg::length2(_data);
            }

            vector3d bulk() const
            {
                return *this;
            }

            T weight() const
            {
                return 0;
            }

            bool operator==(const vector3d &v) const
            {
                return _data == v._data;
            }

            bool operator!=(const vector3d &v) const
            {
                return _data != v._data;
            }

            bool operator==(const linalg::vec<T, 3> &v) const
            {
                return _data == v;
            }

            bool operator!=(const linalg::vec<T, 3> &v) const
            {
                return _data != v;
            }

            vector3d unitized() const
            {
                return vector3d(_data / norm());
            }

            vector3d &_unitize()
            {
                _data /= norm();
                return *this;
            }
        };

        template <class T> class point3d : public vector3d<T>
        {
        public:
            point3d() = default;
            point3d(T x, T y, T z) : vector3d<T>(x, y, z) {}
            point3d(const linalg::vec<T, 3> &v) : vector3d<T>(v) {}

            T weight() const
            {
                return 1;
            }
        };

        template <class T>
        vector3d<T> operator+(const vector3d<T> &a, const vector3d<T> &b)
        {
            return vector3d<T>(a.xyz() + b.xyz());
        }

        template <class T>
        vector3d<T> operator-(const vector3d<T> &a, const vector3d<T> &b)
        {
            return vector3d<T>(a.xyz() - b.xyz());
        }

        template <class T> vector3d<T> operator*(const vector3d<T> &a, T b)
        {
            return vector3d<T>(a.xyz() * b);
        }

        template <class T> vector3d<T> operator*(T a, const vector3d<T> &b)
        {
            return vector3d<T>(a * b.xyz());
        }

        template <class T>
        vector3d<T> operator-(const point3d<T> &a, const point3d<T> &b)
        {
            return vector3d<T>(a.xyz() - b.xyz());
        }

        template <class T>
        point3d<T> operator+(const point3d<T> &a, const vector3d<T> &b)
        {
            return point3d<T>(a.xyz() + b.xyz());
        }

    }
}

#endif