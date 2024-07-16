#ifndef RALGO_SPACE_POSE3_H
#define RALGO_SPACE_POSE3_H

#include <iostream>
#include <nos/trent/trent.h>
#include <ralgo/space/screw.h>

namespace ralgo
{
    template <class T> class pose3
    {
    public:
        linalg::vec<T, 4> ang;
        linalg::vec<T, 3> lin;

        pose3();
        pose3(const linalg::vec<T, 4> &q, const linalg::vec<T, 3> &l);
        pose3(const ralgo::screw<T, 3> &scr);

        pose3(const ralgo::pose3<T> &oth) : ang(oth.ang), lin(oth.lin) {}

        pose3 &operator=(const ralgo::pose3<T> &oth)
        {
            ang = oth.ang;
            lin = oth.lin;
            return *this;
        }

        linalg::vec<T, 3> rotate(linalg::vec<T, 3> vec) const;
        linalg::vec<T, 3> transform(linalg::vec<T, 3> vec) const;
        linalg::vec<T, 3> rotate_vector(linalg::vec<T, 3> vec) const;

        linalg::vec<T, 3> transform_vector(const linalg::vec<T, 3> &vec) const
        {
            return linalg::qrot(ang, vec);
        }

        linalg::vec<T, 3> transform_point(const linalg::vec<T, 3> &vec) const
        {
            return linalg::qrot(ang, vec) + lin;
        }

        linalg::mat<T, 3, 3> rotation_matrix() const
        {
            return {qxdir(ang), qydir(ang), qzdir(ang)};
        }

        linalg::vec<T, 3>
        inverse_transform_vector(const linalg::vec<T, 3> &vec) const
        {
            return linalg::qrot(linalg::qconj(ang), vec);
        }

        linalg::vec<T, 3>
        inverse_transform_point(const linalg::vec<T, 3> &vec) const
        {
            return linalg::qrot(linalg::qconj(ang), vec - lin);
        }

        screw<T, 3> rotate_screw(screw<T, 3> v) const;

        linalg::vec<T, 3> xdir() const;
        linalg::vec<T, 3> ydir() const;
        linalg::vec<T, 3> zdir() const;

        // |A a||B b| = |AB Ab+a|
        // |0 1||0 1|   | 0    1|
        pose3 operator*(const pose3 &oth) const;
        pose3 &operator*=(const pose3 &oth);

        bool operator==(const pose3 &oth) const;

        linalg::vec<T, 3> operator()(linalg::vec<T, 3> vec) const;

        pose3 inverse();

        screw<T, 3> to_screw() const;
        linalg::mat<T, 4, 4> to_mat4() const;

        static pose3 from_screw(const screw<T, 3> &scr);

        static pose3<T> translation(linalg::vec<T, 3> vec);
        __attribute__((deprecated)) static pose3<T>
        euler_rotation(linalg::vec<T, 3> vec);
        static pose3<T> euler(linalg::vec<T, 3> vec);

        static pose3<T> from_trent(const nos::trent &tr)
        {
            auto quat = linalg::vec<T, 4>{(T)tr["ang"][0].as_numer(),
                                          (T)tr["ang"][1].as_numer(),
                                          (T)tr["ang"][2].as_numer(),
                                          (T)tr["ang"][3].as_numer()};
            auto pos = linalg::vec<T, 3>{(T)tr["lin"][0].as_numer(),
                                         (T)tr["lin"][1].as_numer(),
                                         (T)tr["lin"][2].as_numer()};
            return pose3<T>(quat, pos);
        }

        nos::trent to_trent() const
        {
            nos::trent tr;
            tr["ang"].as_list();
            tr["ang"].push_back(ang[0]);
            tr["ang"].push_back(ang[1]);
            tr["ang"].push_back(ang[2]);
            tr["ang"].push_back(ang[3]);
            tr["lin"].as_list();
            tr["lin"].push_back(lin[0]);
            tr["lin"].push_back(lin[1]);
            tr["lin"].push_back(lin[2]);
            return tr;
        }
    };

    template <class T> pose3<T> rot3(linalg::vec<T, 3> vec, T angle);

    template <class T> pose3<T> mov3(linalg::vec<T, 3> vec);
}

using namespace ralgo;

template <class T> ralgo::pose3<T> ralgo::rot3(linalg::vec<T, 3> vec, T angle)
{
    return ralgo::pose3<T>(linalg::rotation_quat<T>(vec, angle), {0, 0, 0});
}

template <class T> ralgo::pose3<T> ralgo::mov3(linalg::vec<T, 3> vec)
{
    return ralgo::pose3<T>({0, 0, 0, 1}, vec);
}

template <class T> ralgo::pose3<T>::pose3() : ang{0, 0, 0, 1}, lin{0, 0, 0} {}

template <class T>
ralgo::pose3<T>::pose3(const linalg::vec<T, 4> &q, const linalg::vec<T, 3> &l)
    : ang(q), lin(l)
{
}

template <class T>
ralgo::pose3<T>::pose3(const ralgo::screw<T, 3> &scr) : lin(scr.lin)
{
    auto w = scr.ang;

    auto angle = length(w);
    auto axis = w / angle;

    if (angle == 0)
        ang = {0, 0, 0, 1};
    else
        ang = rotation_quat(axis, angle);
}

template <class T>
linalg::vec<T, 3> ralgo::pose3<T>::rotate(linalg::vec<T, 3> vec) const
{
    return linalg::qrot(ang, vec);
}

template <class T>
linalg::vec<T, 3> ralgo::pose3<T>::rotate_vector(linalg::vec<T, 3> vec) const
{
    return linalg::qrot(ang, vec);
}

template <class T>
ralgo::screw<T, 3> ralgo::pose3<T>::rotate_screw(screw<T, 3> v) const
{
    return {rotate_vector(v.ang), rotate_vector(v.lin)};
}

template <class T> linalg::vec<T, 3> ralgo::pose3<T>::xdir() const
{
    return linalg::qxdir(ang);
}
template <class T> linalg::vec<T, 3> ralgo::pose3<T>::ydir() const
{
    return linalg::qydir(ang);
}
template <class T> linalg::vec<T, 3> ralgo::pose3<T>::zdir() const
{
    return linalg::qzdir(ang);
}

template <class T>
ralgo::pose3<T> ralgo::pose3<T>::operator*(const ralgo::pose3<T> &oth) const
{
    return ralgo::pose3<T>(linalg::qmul(ang, oth.ang),
                           linalg::qrot(ang, oth.lin) + lin);
}

template <class T>
bool ralgo::pose3<T>::operator==(const ralgo::pose3<T> &oth) const
{
    return lin == oth.lin && ang == oth.ang;
}

template <class T> ralgo::pose3<T> ralgo::pose3<T>::inverse()
{
    auto inv_q = linalg::qinv(ang);

    return {inv_q, -linalg::qrot(inv_q, lin)};
}

template <class T> ralgo::screw<T, 3> ralgo::pose3<T>::to_screw() const
{
    auto axis = linalg::qaxis(ang);
    auto angle = linalg::qangle(ang);

    if (angle == 0)
    {
        axis = {0, 0, 0};
    }

    return {axis * angle, lin};
}

template <class T>
ralgo::pose3<T> ralgo::pose3<T>::from_screw(const ralgo::screw<T, 3> &scr)
{
    auto angle = linalg::length(scr.ang);
    auto axis = scr.ang / angle;

    if (angle == 0)
    {
        axis = {0, 0, 0};
    }

    return {linalg::rotation_quat(axis, angle), scr.lin};
}

template <class T>
ralgo::pose3<T> &ralgo::pose3<T>::operator*=(const ralgo::pose3<T> &oth)
{
    *this = *this * oth;
    return *this;
}

template <class T> linalg::mat<T, 4, 4> ralgo::pose3<T>::to_mat4() const
{
    return {{qxdir(ang), 0}, {qydir(ang), 0}, {qzdir(ang), 0}, {lin, 1}};
}

template <class T>
linalg::vec<T, 3> ralgo::pose3<T>::transform(linalg::vec<T, 3> arg) const
{
    return linalg::qrot(ang, arg) + lin;
}

template <class T>
linalg::vec<T, 3> ralgo::pose3<T>::operator()(linalg::vec<T, 3> arg) const
{
    return transform(arg);
}

template <class T> pose3<T> ralgo::pose3<T>::translation(linalg::vec<T, 3> vec)
{
    return {{0, 0, 0, 1}, vec};
}

template <class T>
pose3<T> ralgo::pose3<T>::euler_rotation(linalg::vec<T, 3> vec)
{
    return pose3<T>{linalg::rotation_quat<T>({0, 0, 1}, vec.z), {}} *
           pose3<T>{linalg::rotation_quat<T>({0, 1, 0}, vec.y), {}} *
           pose3<T>{linalg::rotation_quat<T>({1, 0, 0}, vec.x), {}};
}

template <class T> pose3<T> ralgo::pose3<T>::euler(linalg::vec<T, 3> vec)
{
    // yaw, pitch, roll
    return pose3<T>{linalg::rotation_quat<T>({0, 0, 1}, vec.x), {}} *
           pose3<T>{linalg::rotation_quat<T>({0, 1, 0}, vec.y), {}} *
           pose3<T>{linalg::rotation_quat<T>({1, 0, 0}, vec.z), {}};
}

namespace ralgo
{
    using linalg::ostream_overloads::operator<<;

    template <class T>
    std::ostream &operator<<(std::ostream &os, const pose3<T> &pose)
    {
        return os << "{ang:" << pose.ang << ",lin:" << pose.lin << "}";
    }
}

#endif
