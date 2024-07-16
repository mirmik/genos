#ifndef RALGO_LINALG_EIGEN_H
#define RALGO_LINALG_EIGEN_H

#include <ralgo/poly/poly.h>

namespace ralgo
{
    template <class M> ralgo::poly<typename M::value_type> charpoly(const M &a)
    {
        int dim = a.rows();

        switch (dim)
        {
        case 1:
            return {0};
        case 2:
            return {-a.at(0, 1) * a.at(1, 0), a.at(0, 0) * a.at(1, 1)};
        case 3:
            return {-a.at(0, 1) * a.at(1, 2) * a.at(2, 0) +
                        a.at(0, 2) * a.at(1, 0) * a.at(2, 1) -
                        a.at(0, 0) * a.at(1, 2) * a.at(2, 1),
                    a.at(0, 0) * a.at(1, 1) * a.at(2, 2) -
                        a.at(0, 1) * a.at(1, 0) * a.at(2, 2) +
                        a.at(0, 2) * a.at(1, 1) * a.at(2, 0),
                    -a.at(0, 0) * a.at(1, 2) * a.at(2, 1) +
                        a.at(0, 1) * a.at(1, 2) * a.at(2, 0) +
                        a.at(0, 2) * a.at(1, 0) * a.at(2, 1)};
        default:
            BUG();
        }
    }

    template <class M>
    ralgo::vector<std::complex<double>> eigen_values(const M &a)
    {
        auto char_poly = characteristic_polynomial(a);
        return a.roots();
    }

    template <class M>
    std::vector<ralgo::vector<std::complex<double>>>
    eigen_vectors(const M &a, ralgo::vector<std::complex<double>> evals)
    {
    }
}

#endif