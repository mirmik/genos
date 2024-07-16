#ifndef RALGO_LINALG_QRD_H
#define RALGO_LINALG_QRD_H

/**
    @file
*/

// QR decomposition
// Based on: https://rpubs.com/aaronsc32/qr-decomposition-gram-schmidt

/*
    В процессе ортагонализации Грамма-Шмидта происходит итеративное построение
    нового базиса e на основе старого базиса a согласно принципа:

    b1 = a1
    b2 = a2 - proj(b1, a2)
    b3 = a3 - proj(b1, a3) - proj(u2, a3)         (1)
    ...

    e_i = b_i / ||b_i||   	                      (2)

    Новый базис e оказывается ортонормированным. Ортогонализация достигается
    последовательным исключением компонентов проекций из конструируемого базиса
   (1), а нормирование выполняется непосредственно (2).

    По мере работы алгоритма компоненты векторов e оказываются в матрице Q,
    а множители, восстанавливающие матрицу Q до исходного состояния складываются
    в R.
*/

#include <ralgo/linalg/matops.h>
#include <ralgo/linalg/matrix.h>
#include <ralgo/linalg/trivial_solve.h>

namespace ralgo
{
    /**
     *  QRD разложение или QRD ортоганализация. Выполняется над квадратной
     * матрицей путём последовательного выбора векторов и доворота их до
     * ортогонального состояния. На выходе алгоритма - ортогональная матрица Q и
     * диагональная матрица R.
     */
    template <class M, class TQ, class TR> class QRD_gramschmidt
    {
    public:
        M a = {};
        TQ q = {};
        TR r = {};

        int status = 0;

    public:
        using value_type = typename M::value_type;

        QRD_gramschmidt(const M &_mat) : a(_mat)
        {
            if (_mat.rows() != _mat.rows())
            {
                status = -1;
                return;
            }

            int m = a.rows();
            int n = a.cols();

            q.resize(n, m);
            r.resize(n, n);

            ralgo::matops::clean(q);
            ralgo::matops::clean(r);

            decompose();
        }

        void decompose()
        {
            // int m = a.rows();
            // int n = a.cols();

            ralgo::matops::assign(a, q);

            for (int j = 0; j < a.cols(); ++j)
            {
                // Определяем следующий базисный вектор.
                // b_i = a_i - sum(j=1:i, proj(e_j, a_i) )
                value_type v_data[a.rows()];
                ralgo::vector_view v(v_data, a.rows());
                ralgo::vecops::copy(a.col(j), v);
                for (int i = 0; i < j; ++i)
                {
                    // Ищем операторы проекции.
                    // q содержит компоненты расчитанных ранее векторов:
                    // попутно запоминаем множители. r_ij = e_i * a_j
                    r.at(i, j) =
                        ralgo::vecops::inner_product(q.col(i), a.col(j));

                    // вычитаем слагаемое из компонент вектора.
                    for (unsigned int k = 0; k < v.size(); ++k)
                        v[k] -= r.at(i, j) * q.at(k, i);
                }

                // Устанавливаем диагональные элементы R
                // равными длине векторов v.
                r.at(j, j) = ralgo::vecops::length(v);

                // The orthogonalized result is found and stored
                // in the ith column of Q.
                ralgo::vecops::scalar_div(v, r.at(j, j), q.col(j));
            }
        }

        template <class X, class B> void solve(const B &b, X &&x)
        {
            vector_value_t<B> ybuf[b.size()];
            vector_view y(ybuf, b.size());

            x.resize(b.size());

            // Последовательно применяем решения матриц простого вида.
            // Данная процедура эквивалентна последовательному
            // умножению на обращенные Q, R.
            unary_solve(q, b, y);
            U_triangle_solve(r, y, x);
        }

        template <class X = void, class B> defvec_t<X, B> solve(B &&b)
        {
            defvec_t<X, B> x;
            solve(b, x);
            return x;
        }
    };

    template <class M,
              class Q = ralgo::matrix<typename M::value_type,
                                      ralgo::row_order<typename M::value_type>,
                                      std::allocator<typename M::value_type>>,
              class R = ralgo::matrix<typename M::value_type,
                                      ralgo::row_order<typename M::value_type>,
                                      std::allocator<typename M::value_type>>>
    QRD_gramschmidt<M, Q, R> qrd_gramschmidt(const M &mat)
    {
        return QRD_gramschmidt<M, Q, R>(mat);
    }

    template <class M,
              class Q = ralgo::matrix<typename M::value_type,
                                      ralgo::row_order<typename M::value_type>,
                                      std::allocator<typename M::value_type>>,
              class R = ralgo::matrix<typename M::value_type,
                                      ralgo::row_order<typename M::value_type>,
                                      std::allocator<typename M::value_type>>>
    QRD_gramschmidt<M, Q, R> qrd(const M &mat)
    {
        return QRD_gramschmidt<M, Q, R>(mat);
    }
}

#endif
