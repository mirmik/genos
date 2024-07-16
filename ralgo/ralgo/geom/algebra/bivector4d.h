#ifndef RALGO_GEOMALG_BIVECTOR4D_H
#define RALGO_GEOMALG_BIVECTOR4D_H

#include <ralgo/geom/algebra/bivector3d.h>
#include <ralgo/geom/vector3d.h>

namespace ralgo
{
    namespace geomalg
    {
        template <class T> class bivector4d
        {
            vector3d<T> _v;
            bivector3d<T> _m;

        public:
            bivector4d() = default;
            bivector4d(linalg::vec<T, 3> v, linalg::vec<T, 3> m) : _v(v), _m(m)
            {
            }
            bivector4d(vector3d<T> v, bivector3d<T> m) : _v(v), _m(m) {}
            bivector4d(T vx, T vy, T vz, T mx, T my, T mz)
                : _v(vx, vy, vz), _m(mx, my, mz)
            {
            }

            bivector4d(const bivector4d &other) = default;
            bivector4d &operator=(const bivector4d &other) = default;

            const vector3d<T> &direction() const
            {
                return _v;
            }

            const bivector3d<T> &momentum() const
            {
                return _m;
            }

            const vector3d<T> &v() const
            {
                return _v;
            }

            const bivector3d<T> &m() const
            {
                return _m;
            }

            T vx() const
            {
                return _v.x();
            }

            T vy() const
            {
                return _v.y();
            }

            T vz() const
            {
                return _v.z();
            }

            T mx() const
            {
                return _m.x();
            }

            T my() const
            {
                return _m.y();
            }

            T mz() const
            {
                return _m.z();
            }

            const bivector3d<T> &bulk() const
            {
                return momentum();
            }

            const vector3d<T> &weight() const
            {
                return direction();
            }

            bivector4d unitized() const
            {
                T n = _v.length();
                return bivector4d(_v / n, _m / n);
            }
        };
    }
}

#endif