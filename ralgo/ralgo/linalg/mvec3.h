#ifndef RALGO_LINALG_H
#define RALGO_LINALG_H

#include <ralgo/linalg/linalg.h>

namespace ralgo
{
    template <class T>
    std::pair<T, linalg::vec<T, 3>> vec_mul_vec(const linalg::vec<T, 3> &v0,
                                                const linalg::vec<T, 3> &v1)
    {
        T scalar_part = linalg::dot(v0, v1);
        vec3<T> bivector_part = linalg::cross(v0, v1);
        return std::make_pair(scalar_part, bivector_part);
    }

    template <class T> class mvec3
    {
        T s;                 // []
        linalg::vec<T, 3> v; // e1   e2  e3
        linalg::vec<T, 3> b; // e23  e31 e12
        T t;                 // e321

    public:
        mvec3() = default;
        mvec3(const T &_s,
              const linalg::vec<T, 3> &_v,
              const linalg::vec<T, 3> &_b,
              const T &_t)
            : s(_s), v(_v), b(_b), t(_t)
        {
        }

        mvec3 mul(const mvec3 &oth)
        {
            return mvec3
            {
                s *oth.s + t *oth.t + linalg::dot(v, oth.v) +
                    linalg::dot(b, oth.b);
                s *oth.v + t *oth.b + v *oth.s + b *oth.t;
                s *oth.b + t *oth.v + b *oth.s + v *oth.t;
                s *oth.t + t *oth.s + linalg::dot(v, oth.b) +
                    linalg::dot(b, oth.v);
            };
        }
    }
}

#endif