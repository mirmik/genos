#ifndef RALGO_POLYNOM_H
#define RALGO_POLYNOM_H

#include <ralgo/linalg/roots.h>
#include <ralgo/linalg/vecops.h>
#include <ralgo/linalg/vector.h>

namespace ralgo
{
    template <class T> class poly
    {
    public:
        ralgo::vector<T> coeffs = {};

    public:
        poly() = default;
        poly(const poly &) = default;
        poly(poly &&) = default;
        poly &operator=(const poly &) = default;
        poly &operator=(poly &&) = default;
        poly(const ralgo::vector<T> &coeffs) : coeffs(coeffs) {}
        poly(const std::initializer_list<T> &lst) : coeffs(lst) {}

        explicit poly(size_t n) : poly(ralgo::vector<T>(n)) {}

        size_t degree() const
        {
            return coeffs.size() - 1;
        }

        bool operator==(const poly &oth) const
        {
            return coeffs == oth.coeffs;
        }
        bool operator!=(const poly &oth) const
        {
            return coeffs != oth.coeffs;
        }

        T operator()(T x) const
        {
            double res = 0;
            double x_pow = 1;
            for (unsigned int i = 0; i < coeffs.size(); ++i)
            {
                res += coeffs[i] * x_pow;
                x_pow *= x;
            }
            return res;
        }

        poly<T> derivative() const
        {
            ralgo::vector<double> res(coeffs.size() - 1);
            for (unsigned int i = 1; i < coeffs.size(); ++i)
            {
                res[i - 1] = coeffs[i] * i;
            }
            return res;
        }

        poly<T> integral() const
        {
            ralgo::vector<double> res(coeffs.size() + 1);
            res[0] = 0;
            for (unsigned int i = 0; i < coeffs.size(); ++i)
            {
                res[i + 1] = coeffs[i] / (i + 1);
            }
            return res;
        }

        poly<T> operator+(const poly<T> &other) const
        {
            ralgo::vector<T> res(coeffs.size());
            for (unsigned int i = 0; i < res.size(); ++i)
            {
                res[i] = coeffs[i] + other.coeffs[i];
            }
            return res;
        }

        poly<T> operator-(const poly<T> &other) const
        {
            ralgo::vector<T> res(coeffs.size());
            for (unsigned int i = 0; i < res.size(); ++i)
            {
                res[i] = coeffs[i] - other.coeffs[i];
            }
            return res;
        }

        poly<T> operator*(const poly<T> &other) const
        {
            ralgo::vector<T> res(coeffs.size() + other.coeffs.size() - 1);
            ralgo::vecops::clean(res);

            for (unsigned int i = 0; i < coeffs.size(); ++i)
            {
                for (unsigned int j = 0; j < other.coeffs.size(); ++j)
                {
                    res[i + j] += coeffs[i] * other.coeffs[j];
                }
            }
            return res;
        }

        std::pair<poly<T>, poly<T>> div(const poly<T> &b) const
        {
            ralgo::poly<T> q(degree() - b.degree() + 1);
            ralgo::poly<T> r;

            ralgo::vecops::copy(coeffs, r.coeffs);

            for (int i = q.degree(); i >= 0; --i)
            {
                q.coeffs[i] = r.coeffs[i + b.degree()] / b.coeffs[b.degree()];
                for (int j = b.coeffs.size() - 1; j >= 0; --j)
                {
                    r.coeffs[i + j] -= q.coeffs[i] * b.coeffs[j];
                }
            }

            r.coeffs.resize(b.degree());
            r.strip();
            return {q, r};
        }

        void strip()
        {
            int deg = degree();

            for (int i = degree(); i >= 0; --i)
            {
                if (coeffs[i] != 0)
                {
                    deg = i;
                    break;
                }
            }

            coeffs.resize(deg + 1);
        }

        poly<T> gcd(const poly<T> &b) const
        {
            poly<T> r1 = *this;
            poly<T> r2 = b;

            while (r1.degree() > 0 && r2.degree() > 0)
            {
                if (r1.degree() > r2.degree())
                {
                    r1 = r1.div(r2).second;
                }
                else
                {
                    r2 = r2.div(r1).second;
                }
            }

            if (r1.degree() == 0)
                return r2;

            else
                return r1;
        }

        std::vector<T> roots() const
        {
            return ralgo::roots(coeffs);
        }
    };

    static inline std::ostream &operator<<(std::ostream &os,
                                           const ralgo::poly<double> &p)
    {
        return os << p.coeffs;
    }
}

#endif