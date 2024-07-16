#ifndef RALGO_GEOM_ZONE_CHECK_H
#define RALGO_GEOM_ZONE_CHECK_H

#include <ralgo/linalg/linalg.h>

namespace ralgo
{
    template <class Pnt, class Vec>
    bool point2_in_polygon(const Vec &psegm, const Pnt &t)
    {
        assert(std::size(psegm) >= 3);

        int lastsign = 0;
        for (unsigned int i = 0; i < std::size(psegm); ++i)
        {
            unsigned int s = i == std::size(psegm) - 1 ? 0 : i + 1;

            auto &a = psegm[i];
            auto &b = psegm[s];

            auto ba = b - a;
            auto ta = t - a;

            int sign = linalg::cross(ba, ta) >= 0 ? +1 : -1;

            if (lastsign != 0 && lastsign != sign)
                return false;

            lastsign = sign;
        }
        return true;
    }
}

#endif
