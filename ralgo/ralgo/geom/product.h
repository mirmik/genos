#ifndef RALGO_GEOM_PRODUCT_H
#define RALGO_GEOM_PRODUCT_H

#include <ralgo/geom/vector2d.h>
#include <ralgo/geom/vector3d.h>

namespace ralgo
{
    namespace geom
    {
        template <class T> T dot(const vector2d<T> &a, const vector2d<T> &b)
        {
            return a.x() * b.x() + a.y() * b.y();
        }

        template <class T> T dot(const vector3d<T> &a, const vector3d<T> &b)
        {
            return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
        }

        template <class T>
        vector3d<T> cross(const vector3d<T> &a, const vector3d<T> &b)
        {
            return vector3d<T>(a.y() * b.z() - a.z() * b.y(),
                               a.z() * b.x() - a.x() * b.z(),
                               a.x() * b.y() - a.y() * b.x());
        }
    }
}

#endif