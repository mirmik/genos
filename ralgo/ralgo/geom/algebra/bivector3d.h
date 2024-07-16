#ifndef RALGO_GEOMALG_BIVECTOR3D_H
#define RALGO_GEOMALG_BIVECTOR3D_H

#include <ralgo/geom/algebra/base.h>
#include <ralgo/linalg/linalg.h>

namespace ralgo
{
    namespace geomalg
    {
        template <class T> class bivector3d
        {
            linalg::vec<T, 3> _data;

        public:
            bivector3d() = default;
            bivector3d(T x, T y, T z) : _data(x, y, z) {}
            bivector3d(const linalg::vec<T, 3> &v) : _data(v) {}
            bivector3d(const bivector3d &v) = default;
            bivector3d &operator=(const bivector3d &v) = default;

            const linalg::vec<T, 3> &xyz()
            {
                return _data;
            }

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

            bool operator==(const bivector3d &v) const
            {
                return _data == v._data;
            }

            bool operator!=(const bivector3d &v) const
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

            T norm() const
            {
                return linalg::length(_data);
            }

            T norm2() const
            {
                return linalg::length2(_data);
            }
        };
    }
}

#endif