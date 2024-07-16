#ifndef RALGO_GEOMPAR_PAREQUATION3D_H
#define RALGO_GEOMPAR_PAREQUATION3D_H

#include <ralgo/geom/vector2d.h>
#include <ralgo/geom/vector3d.h>

namespace ralgo
{
    namespace geompar
    {
        template <class T> class equation1p2d
        {
        public:
            virtual ralgo::geom::point2d<T> d0(double u) const = 0;
            virtual ralgo::geom::vector2d<T> d1(double u) const = 0;
            virtual std::pair<T, T> urange() const = 0;
        };

        template <class T> class equation1p3d
        {
        public:
            virtual ralgo::geom::point3d<T> d0(double u) const = 0;
            virtual ralgo::geom::vector3d<T> d1(double u) const = 0;
            virtual std::pair<T, T> urange() const = 0;
        };

        template <class T> class equation2p2d
        {
        public:
            virtual ralgo::geom::point2d<T> d0(double u, double v) const = 0;
            virtual ralgo::geom::vector2d<T> d1(double u, double v) const = 0;
            virtual std::pair<T, T> urange() const = 0;
            virtual std::pair<T, T> vrange() const = 0;
        };

        template <class T> class equation2p3d
        {
        public:
            virtual ralgo::geom::point3d<T> d0(double u, double v) const = 0;
            virtual ralgo::geom::vector3d<T> d1(double u, double v) const = 0;
            virtual std::pair<T, T> urange() const = 0;
            virtual std::pair<T, T> vrange() const = 0;
        };
    }
}

#endif