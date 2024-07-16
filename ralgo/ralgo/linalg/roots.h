#ifndef RALGO_ROOTS_H
#define RALGO_ROOTS_H

#include <cmath>
#include <ralgo/linalg/roots_finder.h>
#include <ralgo/linalg/vector.h>
#include <vector>

namespace ralgo
{
    template <class T> ralgo::vector<T> roots(T b, T a)
    {
        return {-b / a};
    }

    template <class T> ralgo::vector<T> roots(T c, T b, T a)
    {
        double d = b * b - 4 * a * c;
        if (d < 0)
        {
            return {};
        }

        double sqrt_d = std::sqrt(d);
        return {(-b - sqrt_d) / (2 * a), (-b + sqrt_d) / (2 * a)};
    }

    static inline ralgo::vector<double> roots(ralgo::vector<double> coeffs)
    {
        int degree = coeffs.size() - 1;
        if (coeffs.size() < 2)
        {
            return {};
        }
        if (coeffs.size() == 2)
        {
            return roots(coeffs[0], coeffs[1]);
        }
        if (coeffs.size() == 3)
        {
            return roots(coeffs[0], coeffs[1], coeffs[2]);
        }

        PolynomialRootFinder finder;

        int ret[2];
        ralgo::vector<double> real_roots(degree);
        ralgo::vector<double> imag_roots(degree);
        finder.FindRoots(
            coeffs.data(), degree, real_roots.data(), imag_roots.data(), ret);
        return real_roots;
    }
}

#endif