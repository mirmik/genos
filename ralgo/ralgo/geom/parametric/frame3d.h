#ifndef RALGO_GEOMPAR_FRAME3D_H
#define RALGO_GEOMPAR_FRAME3D_H

#include <ralgo/geom/vector3d.h>
#include <ralgo/space/pose3.h>

namespace ralgo
{
    namespace geompar
    {
        template <class T> class frame3d
        {
            ralgo::geom::point3d<T> _origin;
            ralgo::geom::vector3d<T> _xaxis;
            ralgo::geom::vector3d<T> _yaxis;
            ralgo::geom::vector3d<T> _zaxis;

        public:
            frame3d() = default;
            frame3d(const ralgo::geom::point3d<T> &origin,
                    const ralgo::geom::vector3d<T> &xaxis,
                    const ralgo::geom::vector3d<T> &yaxis,
                    const ralgo::geom::vector3d<T> &zaxis)
                : _origin(origin), _xaxis(xaxis), _yaxis(yaxis), _zaxis(zaxis)
            {
            }

            ralgo::geom::point3d<T> origin() const
            {
                return _origin;
            }

            ralgo::geom::vector3d<T> xaxis() const
            {
                return _xaxis;
            }

            ralgo::geom::vector3d<T> yaxis() const
            {
                return _yaxis;
            }

            ralgo::geom::vector3d<T> zaxis() const
            {
                return _zaxis;
            }

            ralgo::geom::point3d<T> d0(const linalg::vec<T, 3> &v) const
            {
                return _origin + _xaxis * v[0] + _yaxis * v[1] + _zaxis * v[2];
            }

            ralgo::geom::point3d<T> d0(T x, T y, T z) const
            {
                return _origin + _xaxis * x + _yaxis * y + _zaxis * z;
            }
        };
    }
}

#endif