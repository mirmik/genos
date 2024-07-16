#ifndef RALGO_GEOMALG_PLANE_H
#define RALGO_GEOMALG_PLANE_H

#include <ralgo/geom/vector3d.h>

namespace ralgo
{
    namespace geompar
    {
        template <class T> class trivector4d
        {
            ralgo::geom::point3d<T> _origin;
            ralgo::geom::vector3d<T> _xaxis;
            ralgo::geom::vector3d<T> _yaxis;

        public:
            trivector4d() = default;
            trivector4d(const ralgo::geom::point3d<T> &origin,
                        const ralgo::geom::vector3d<T> &xaxis,
                        const ralgo::geom::vector3d<T> &yaxis)
                : _origin(origin), _xaxis(xaxis), _yaxis(yaxis)
            {
            }
        };
    }
}

#endif