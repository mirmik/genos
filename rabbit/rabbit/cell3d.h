#ifndef RABBIT_CELL3D_H
#define RABBIT_CELL3D_H

#include <rabbit/types.h>

namespace rabbit
{

    class cell3d
    {
    public:
        linalg::vec<float, 3> ll = {};
        linalg::vec<float, 3> hl = {};
        linalg::vec<float, 3> lh = {};
        linalg::vec<float, 3> hh = {};

    public:
        cell3d() = default;
        cell3d(const linalg::vec<float, 3> &ll,
               const linalg::vec<float, 3> &hl,
               const linalg::vec<float, 3> &lh,
               const linalg::vec<float, 3> &hh)
            : ll(ll), hl(hl), lh(lh), hh(hh)
        {
        }
    };
} // namespace rabbit

#endif