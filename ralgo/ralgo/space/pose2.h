#ifndef RALGO_GEOM_POSE2_H
#define RALGO_GEOM_POSE2_H

#include <ralgo/linalg/linalg.h>
#include <ralgo/space/screw.h>

namespace ralgo
{
    template <class T> class pose2
    {
    public:
        T ang;
        linalg::vec<T, 2> lin;

    public:
        pose2() : ang{}, lin{} {}

        pose2(T ang, linalg::vec<T, 2> lin) : ang(ang), lin(lin) {}

        pose2 &operator=(const pose2 &pos)
        {
            ang = pos.ang;
            lin = pos.lin;

            return *this;
        }

        pose2 inverse()
        {
            return {-ang, linalg::rot<T>(-ang, -lin)};
        }

        pose2 operator*(const pose2 &oth)
        {
            return pose2(ang + oth.ang, linalg::rot<T>(ang, oth.lin) + lin);
        }

        /*pose2 operator() (const pose2& oth)
        {
            return (*this)(oth);
        }*/

        linalg::vec<T, 2> transform_vector(const linalg::vec<T, 2> &vec) const
        {
            return linalg::rot(ang, vec);
        }

        linalg::vec<T, 2> transform_point(const linalg::vec<T, 2> &vec) const
        {
            return linalg::rot(ang, vec) + lin;
        }

        linalg::vec<T, 2>
        inverse_transform_vector(const linalg::vec<T, 2> &vec) const
        {
            return linalg::rot(-ang, vec);
        }

        linalg::vec<T, 2>
        inverse_transform_point(const linalg::vec<T, 2> &vec) const
        {
            return linalg::rot(-ang, vec - lin);
        }

        ralgo::screw<T, 2> rotate(ralgo::screw<T, 2> scr) const
        {
            return {scr.ang, rotate(scr.lin)};
        }

        ralgo::screw<T, 2> rotate_screw(ralgo::screw<T, 2> scr) const
        {
            return {scr.ang, rotate(scr.lin)};
        }

        linalg::vec<T, 2> rotate(linalg::vec<T, 2> vec) const
        {
            return linalg::rot(ang, vec);
        }

        linalg::vec<T, 2> translation()
        {
            return lin;
        }
    };

    template <class T> T vector_angle(linalg::vec<T, 2> vec)
    {
        return atan2(vec.y, vec.x);
    }
}

namespace ralgo
{
    using linalg::ostream_overloads::operator<<;

    template <class T>
    std::ostream &operator<<(std::ostream &os, const pose2<T> &pose)
    {
        return os << "{ang:" << pose.ang << ",lin:" << pose.lin << "}";
    }
}

#endif
