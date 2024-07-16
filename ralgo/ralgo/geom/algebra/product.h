#ifndef RALGO_GEOMALG_PRODUCT_H
#define RALGO_GEOMALG_PRODUCT_H

#include <ralgo/geom/algebra/base.h>
#include <ralgo/geom/algebra/complement.h>
#include <ralgo/geom/product.h>

namespace ralgo
{
    namespace geomalg
    {
        template <class T>
        bivector4d<T> wedge4d(const vector4d<T> &p, const vector4d<T> &q)
        {
            return bivector4d(p.w() * q.x() - p.x() * q.w(),
                              p.w() * q.y() - p.y() * q.w(),
                              p.w() * q.z() - p.z() * q.w(),
                              p.y() * q.z() - p.z() * q.y(),
                              p.z() * q.x() - p.x() * q.z(),
                              p.x() * q.y() - p.y() * q.x());
        }

        template <class T>
        bivector4d<T> wedge4d(const point3d<T> &p, const point3d<T> &q)
        {
            return bivector4d(q.x() - p.x(),
                              q.y() - p.y(),
                              q.z() - p.z(),
                              p.y() * q.z() - p.z() * q.y(),
                              p.z() * q.x() - p.x() * q.z(),
                              p.x() * q.y() - p.y() * q.x());
        }

        template <class T>
        bivector4d<T> wedge4d(const point3d<T> &p, const vector3d<T> &v)
        {
            return bivector4d(v.x(),
                              v.y(),
                              v.z(),
                              p.y() * v.z() - p.z() * v.y(),
                              p.z() * v.x() - p.x() * v.z(),
                              p.x() * v.y() - p.y() * v.x());
        }

        template <class T>
        trivector4d<T> wedge4d(const bivector4d<T> &l, const vector4d<T> &p)
        {
            double x = l.vy() * p.z() - l.vz() * p.y() + l.mx() * p.w();
            double y = l.vz() * p.x() - l.vx() * p.z() + l.my() * p.w();
            double z = l.vx() * p.y() - l.vy() * p.x() + l.mz() * p.w();
            double w = -(l.mx() * p.x() + l.my() * p.y() + l.mz() * p.z());
            return trivector4d(x, y, z, w);
        }

        template <class T>
        trivector4d<T> wedge4d(const bivector4d<T> &l, const point3d<T> &p)
        {
            double x = l.vy() * p.z() - l.vz() * p.y() + l.mx();
            double y = l.vz() * p.x() - l.vx() * p.z() + l.my();
            double z = l.vx() * p.y() - l.vy() * p.x() + l.mz();
            double w = -(l.mx() * p.x() + l.my() * p.y() + l.mz() * p.z());
            return trivector4d(x, y, z, w);
        }

        template <class T>
        bivector4d<T> antiwedge4d(const trivector4d<T> &g,
                                  const trivector4d<T> &h)
        {
            linalg::vec<T, 3> v(g.z() * h.y() - g.y() * h.z(),
                                g.x() * h.z() - g.z() * h.x(),
                                g.y() * h.x() - g.x() * h.y());
            linalg::vec<T, 3> m(g.x() * h.w() - g.w() * h.x(),
                                g.y() * h.w() - g.w() * h.y(),
                                g.z() * h.w() - g.w() * h.z());
            return bivector4d(v, m);
        }

        template <class T>
        point4d<T> antiwedge4d(const bivector4d<T> &l, const trivector4d<T> &g)
        {
            double x = l.my() * g.z() - l.mz() * g.y() + l.vx() * g.w();
            double y = l.mz() * g.x() - l.mx() * g.z() + l.vy() * g.w();
            double z = l.mx() * g.y() - l.my() * g.x() + l.vz() * g.w();
            double w = -(l.vx() * g.x() + l.vy() * g.y() + l.vz() * g.z());
            return {x, y, z, w};
        }
    }
}

#endif