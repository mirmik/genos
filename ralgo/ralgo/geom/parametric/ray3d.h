#ifndef RALGO_GEOMPAR_RAY_H
#define RALGO_GEOMPAR_RAY_H

#include <ralgo/geom/parametric/parequation3d.h>
#include <ralgo/geom/vector3d.h>

namespace ralgo
{
    namespace geompar
    {
        template <class T> class ray3d : public equation1p3d<T>
        {
            ralgo::geom::point3d<T> _origin;
            ralgo::geom::vector3d<T> _direction;

        public:
            ray3d() = default;
            ray3d(const ralgo::geom::point3d<T> &origin,
                  const ralgo::geom::vector3d<T> &direction)
                : _origin(origin), _direction(direction)
            {
            }

            static ray3d from_points(const ralgo::geom::point3d<T> &p1,
                                     const ralgo::geom::point3d<T> &p2)
            {
                return ray3d(p1, p2 - p1);
            }

            const ralgo::geom::point3d<T> &origin() const
            {
                return _origin;
            }

            const ralgo::geom::vector3d<T> &direction() const
            {
                return _direction;
            }

            void _unitize()
            {
                _direction._unitize();
            }

            ray3d unitized() const
            {
                return ray3d(_origin, _direction.unitized());
            }

            ralgo::geom::point3d<T> d0(double u) const override
            {
                return _origin + _direction * u;
            }

            ralgo::geom::vector3d<T> d1(double u) const override
            {
                (void)u;
                return _direction;
            }

            std::pair<T, T> urange() const override
            {
                return {std::numeric_limits<T>::lowest(),
                        std::numeric_limits<T>::max()};
            }
        };
    }
}

#endif