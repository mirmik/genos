#ifndef RALGO_LINALG_LU_DECOMPOSITION_H
#define RALGO_LINALG_LU_DECOMPOSITION_H

/**
    @file
*/

#include <ralgo/linalg/matops.h>
#include <ralgo/linalg/matrix.h>
#include <ralgo/linalg/proxy.h>
#include <ralgo/linalg/trivial_inverse.h>
#include <ralgo/linalg/trivial_solve.h>

#include <nos/print.h>

namespace ralgo
{
    /**
     *  Разложение квадратной матрицы A на верхнедиагональную U и
     * нижнедиагональную L. P - вспомогательная перестановочная матрица.
     *
     *  PLUD разложение является формальным-аналогом метода Гаусса.
     */
    template <class M, class TP, class TL, class TU> class PLUD
    {
    public:
        M a = {};
        TP p = {};
        TL l = {};
        TU u = {};

        int status = 0;
        int swap_counter = 0;

        PLUD(const M &_mat) : PLUD(_mat, TP{}, TL{}, TU{}) {}

        PLUD(const M &_mat, const TP &_p, const TL &_l, const TU &_u)
            : a(_mat), p(_p), l(_l), u(_u)
        {
            if (_mat.rows() != _mat.cols())
            {
                status = -1;
                return;
            }

            int n = a.rows();

            p.resize(n, n);
            l.resize(n, n);
            u.resize(n, n);

            ralgo::matops::clean(p);
            ralgo::matops::clean(l);
            ralgo::matops::clean(u);

            ralgo::matops::eye(p);
            ralgo::matops::eye(l);
            ralgo::matops::assign(a, u);

            decompose();
        }

        void decompose()
        {
            const int n = a.rows();
            swap_counter = 0;

            for (int i = 0; i < n; i++)
            {
                double pivotValue = 0;
                int pivot = -1;
                for (int row = i; row < n; row++)
                {
                    if (std::abs(u(row, i)) > pivotValue)
                    {
                        pivotValue = std::abs(u[row][i]);
                        pivot = row;
                    }
                }
                if (pivotValue != 0)
                {
                    if (pivot != i)
                    {
                        ralgo::vecops::swap(p.row(pivot), p.row(i));
                        ralgo::vecops::swap(u.row(pivot), u.row(i));
                        swap_counter++;
                    }

                    for (int j = i + 1; j < n; j++)
                    {
                        u(j, i) /= u(i, i);
                        for (int k = i + 1; k < n; k++)
                            u(j, k) -= u(j, i) * u(i, k);
                    }
                }
            }

            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < i; j++)
                {
                    l(i, j) = u(i, j);
                    u(i, j) = 0;
                }
            }

            ralgo::matops::square_inline_transpose(p);
        }

        template <class X, class B> void solve(const B &b, X &&x)
        {
            std::vector<vector_value_t<B>> ybuf(b.size());
            vector_view y(ybuf.data(), b.size());

            x.resize(b.size());

            // Последовательно применяем решения матриц простого вида.
            // Данная процедура эквивалентна последовательному
            // умножению на обращенные P, L, U.
            // details : x в первом выражении используется как промежуточное
            // хранилище.
            pivot_solve(p, b, x);
            L_triangle_solve(l, x, y);
            U_triangle_solve(u, y, x);
        }

        template <class X = void, class B> defvec_t<X, B> solve(B &&b)
        {
            defvec_t<X, B> x;
            solve(b, x);
            return x;
        }

        /// A_inv = U_inv * L_inv * P_inv
        void inverse(M &inv) const
        {
            ralgo::matrix<typename M::value_type> temp;
            TL l_inv = L_triangle_inverse(l);
            TU u_inv = U_triangle_inverse(u);
            auto p_inv = ralgo::transposed_matrix_proxy(p);
            ralgo::matops::multiply(u_inv, l_inv, temp);
            ralgo::matops::multiply(temp, p_inv, inv);
        }

        typename M::value_type determinant() const
        {
            auto lprod = ralgo::matops::diagprod(l);
            auto uprod = ralgo::matops::diagprod(u);
            int pivot_det = swap_counter % 2 == 0 ? 1 : -1;
            return lprod * uprod * pivot_det;
        }

        M inverse() const
        {
            ralgo::matrix<typename M::value_type> inv;
            inverse(inv);
            return inv;
        }
    };

    template <class M,
              class P = ralgo::matrix<typename M::value_type,
                                      ralgo::row_order<typename M::value_type>,
                                      std::allocator<typename M::value_type>>,
              class L = ralgo::matrix<typename M::value_type,
                                      ralgo::row_order<typename M::value_type>,
                                      std::allocator<typename M::value_type>>,
              class U = ralgo::matrix<typename M::value_type,
                                      ralgo::row_order<typename M::value_type>,
                                      std::allocator<typename M::value_type>>>
    PLUD<M, P, L, U> plud(const M &mat)
    {
        return PLUD<M, P, L, U>(mat);
    }

    template <class M,
              class P = ralgo::matrix<typename M::value_type,
                                      ralgo::row_order<typename M::value_type>,
                                      std::allocator<typename M::value_type>>,
              class L = ralgo::matrix<typename M::value_type,
                                      ralgo::row_order<typename M::value_type>,
                                      std::allocator<typename M::value_type>>,
              class U = ralgo::matrix<typename M::value_type,
                                      ralgo::row_order<typename M::value_type>,
                                      std::allocator<typename M::value_type>>>
    PLUD<M, P, L, U> plud(const M &mat, const P &p, const L &l, const U &u)
    {
        return PLUD<M, P, L, U>(mat, p, l, u);
    }
}

#endif
