#ifndef RALGO_LINALG_MVEC301_H
#define RALGO_LINALG_MVEC301_H

namespace ralgo
{
    template <class T> class mvec301
    {
        T s;                 // []
        linalg::vec<T, 3> v; // e1   e2   e3
        linalg::vec<T, 3> b; // e23  e31  e12
        T t;                 // e321

        T ds;                 // e4
        linalg::vec<T, 3> dv; // e14  e24  e34
        linalg::vec<T, 3> db; // e234  e314  e124
        T dt;                 // e3214

    public:
        mvec301() = default;
        mvec301(const T &_s,
                const linalg::vec<T, 3> &_v,
                const linalg::vec<T, 3> &_b,
                const T &_t)
            : s(_s), v(_v), b(_b), t(_t)
        {
        }

        mvec301 mul(const mvec301 &oth)
        {
            return mvec3{s * oth.s + t * oth.t + linalg::dot(v, oth.v) +
                             linalg::dot(b, oth.b),
                         s * oth.v + t * oth.b + v * oth.s + b * oth.t,
                         s * oth.b + t * oth.v + b * oth.s + v * oth.t,
                         s * oth.t + t * oth.s + linalg::dot(v, oth.b) +
                             linalg::dot(b, oth.v)};
        }
    }

    class rvec301
    {
        T s;                  // []
        linalg::vec<T, 3> b;  // e23  e31  e12
        linalg::vec<T, 3> dv; // e14  e24  e34
        T dt;                 // e3214

        rvec301 mul(const rvec301 &oth)
        {
            return rvec301{s * oth.s + linalg::dot(b, oth.b) + 0 + 0,
                           s * oth.b + b * oth.s +
                               linalg::cross(b, oth.b) s * oth.dv + dv * oth.s +
                               s * oth.dt + linalg::dot(b, oth.dv) +
                               linalg::dot(oth.dv, b) + };
        }
    }
}

#endif