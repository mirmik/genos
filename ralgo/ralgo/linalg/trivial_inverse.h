#ifndef RALGO_LINALG_TRIANGLE_INVERSE_H
#define RALGO_LINALG_TRIANGLE_INVERSE_H

#include <ralgo/linalg/matops.h>
#include <ralgo/linalg/matrix.h>
#include <ralgo/util/helpers.h>

namespace ralgo
{

    // Обратная для верхнетреугольной матрицы
    template <class R, class A> void U_triangle_inverse(const A &a, R &inv)
    {
        inv.resize(a.rows(), a.cols());
        ralgo::matops::eye(inv);

        for (int i = a.rows() - 1; i >= 0; --i)
        {
            double inv_diag_elem = 1.0 / a.at(i, i);
            for (int k = i; k < a.cols(); ++k)
            {
                inv.at(i, k) *= inv_diag_elem;
            }

            for (int j = i - 1; j >= 0; --j)
            {
                double mul = a.at(j, i);
                for (int k = i; k < a.cols(); ++k)
                {
                    inv.at(j, k) -= mul * inv.at(i, k);
                }
            }
        }
    }

    template <class R = void, class M>
    defsame_t<R, ralgo::matrix<typename M::value_type>>
    U_triangle_inverse(const M &a)
    {
        defsame_t<R, ralgo::matrix<typename M::value_type>> inv;
        U_triangle_inverse(a, inv);
        return inv;
    }

    // Обратная для нижнеугольной матрицы
    template <class R, class A> void L_triangle_inverse(const A &a, R &inv)
    {
        inv.resize(a.rows(), a.cols());
        ralgo::matops::eye(inv);

        for (int i = 0; i < a.rows(); ++i)
        {
            double inv_diag_elem = 1.0 / a.at(i, i);
            for (int k = 0; k <= i; ++k)
            {
                inv.at(i, k) *= inv_diag_elem;
            }

            for (int j = i + 1; j < a.rows(); ++j)
            {
                double mul = a.at(j, i);
                for (int k = 0; k <= i; ++k)
                {
                    inv.at(j, k) -= mul * inv.at(i, k);
                }
            }
        }
    }

    template <class R = void, class M>
    defsame_t<R, ralgo::matrix<typename M::value_type>>
    L_triangle_inverse(const M &a)
    {
        defsame_t<R, ralgo::matrix<typename M::value_type>> inv;
        L_triangle_inverse(a, inv);
        return inv;
    }
}

#endif