#ifndef RALGO_GEOM_TO_ALGEBRAIC_H
#define RALGO_GEOM_TO_ALGEBRAIC_H

#include <ralgo/geom/algebra/join.h>
#include <ralgo/geom/parametric/ray3d.h>

namespace ralgo
{
    namespace geom
    {
        template <class T>
        ralgo::geomalg::bivector4d<T>
        to_algebraic(const ralgo::geompar::ray3d<T> &r)
        {
            return ralgo::geomalg::join(r.origin(), r.origin() + r.direction());
        }
    }
}

#endif