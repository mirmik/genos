#ifndef RALGO_INTERPOLATE_H
#define RALGO_INTERPOLATE_H

#include <cmath>
#include <iterator>
#include <vector>

namespace ralgo
{
    template <class V, class C>
    V bilinear_interpolation(const C &rkoeff,
                             const C &ckoeff,
                             const V &lb,
                             const V &rb,
                             const V &lt,
                             const V &rt)
    {
        return (1 - rkoeff) * (1 - ckoeff) * lb + (rkoeff) * (1 - ckoeff) * rb +
               (1 - rkoeff) * (ckoeff)*lt + (rkoeff) * (ckoeff)*rt;
    }

    template <class A, class B, class K> auto lerp(A a, B b, K k)
    {
        return a * (1 - k) + b * k;
    }

    template <class A, class B, class U>
    auto lerpcoeff(A left, B right, U target)
        -> decltype((target - left) / (right - left))
    {
        if (std::isinf(left))
        {
            return 1;
        }

        if (std::isinf(right))
        {
            return 0;
        }

        if (left == right)
        {
            return 0;
        }

        return (target - left) / (right - left);
    }

    template <class T> class linspace
    {
        class linspace_iterator
            : public std::
                  iterator<std::bidirectional_iterator_tag, T, T, T *, T &>
        {
            linspace *ls;
            int p;

        public:
            linspace_iterator(linspace *ls, int p) : ls(ls), p(p) {}
            T operator*()
            {
                return ls->operator[](p);
            }
            linspace_iterator &operator++()
            {
                ++p;
                return *this;
            }

            bool operator==(const linspace_iterator &oth) const
            {
                return p == oth.p && ls == oth.ls;
            }
            bool operator!=(const linspace_iterator &oth) const
            {
                return p != oth.p || ls != oth.ls;
            }
        };

        T a, b;
        int points;

    public:
        linspace(T _a, T _b, int _points, bool endpoint = true)
            : a(_a), b(_b), points(_points)
        {
            if (points < 2)
                points = 2;

            if (!endpoint)
            {
                double k = (double)(points - 1) / (double)(points);
                b = lerp(a, b, k);
            }
        }

        T step()
        {
            return (b - a) / (points - 1);
        }

        T operator[](int p)
        {
            double koeff = (double)p / (double)(points - 1);
            return lerp(a, b, koeff);
        }

        int size() const
        {
            return points;
        }

        linspace_iterator begin()
        {
            return linspace_iterator(this, 0);
        }
        linspace_iterator end()
        {
            return linspace_iterator(this, points);
        }

        std::vector<T> to_vector()
        {
            std::vector<T> ret(points);
            for (int i = 0; i < points; i++)
            {
                ret[i] = operator[](i);
            }
            return ret;
        }
    };
}

#endif
