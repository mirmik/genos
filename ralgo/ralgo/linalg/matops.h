#ifndef RALGO_MATOPS_H
#define RALGO_MATOPS_H

/**
    @file
*/

#include <iterator>
#include <utility>

#include <igris/container/array_view.h>
#include <ralgo/linalg/vecops.h>
#include <ralgo/linalg/vecops_base.h>
#include <ralgo/util/helpers.h>

#include <ralgo/linalg/matrix.h>
#include <ralgo/linalg/vector.h>

// Алгоритмы для матриц имеющих интерфейс схожий с интерфейсом boost.matrix
namespace ralgo
{
    namespace matops
    {
        template <class M> void fill(M &arr, const value_t<M> &val)
        {
            vecops::fill(arr, val);
        }

        template <class M> void clean(M &arr)
        {
            vecops::fill(arr, typename M::value_type{});
        }

        template <class M> typename M::value_type diagprod(M &a)
        {
            typename M::value_type acc = 1;
            for (auto i = 0; i < a.rows(); i++)
            {
                acc *= a.at(i, i);
            }
            return acc;
        }

        template <class M> typename M::value_type trace(M &a)
        {
            typename M::value_type acc = 0;
            for (auto i = 0; i < a.rows(); i++)
            {
                acc += a.at(i, i);
            }
            return acc;
        }

        template <class M> void square_inline_transpose(M &a)
        {
            for (int i = 0; i < a.rows(); ++i)
            {
                for (int j = i + 1; j < a.rows(); ++j)
                {
                    std::swap(a.at(i, j), a.at(j, i));
                }
            }
        }

        template <class T> auto fill(int r, int c, T val) -> matrix<T>
        {
            matrix<T> ret(r, c);
            vecops::fill(ret, val);
            return ret;
        }

        template <class A, class B> void copy(A &a, const B &b)
        {
            size_t m = a.size1();
            size_t n = a.size2();

            assert(a.size1() == b.size1());
            assert(a.size2() == b.size2());

            for (unsigned int i = 0; i < m; ++i)
                for (unsigned int j = 0; j < n; ++j)
                {
                    a(i, j) = b(i, j);
                }
        }

        template <class M, class VI, class VEI>
        void copy_from_rows(M &tgt, VI srcit, VEI eit)
        {
            size_t n = std::size(*srcit);
            size_t m = std::distance(srcit, eit);

            for (unsigned int i = 0; i < m; ++i)
            {
                auto &v = *srcit;
                for (unsigned int j = 0; j < n; ++j)
                {
                    tgt(i, j) = v[j];
                }
                srcit++;
            }
        }
        template <class M, class VV> void copy_from_rows(M &tgt, const VV &vecs)
        {
            copy_from_rows(tgt, std::cbegin(vecs), std::cend(vecs));
        }

        template <class M, class VI, class VEI>
        void copy_from_cols(M &tgt, VI srcit, VEI eit)
        {
            size_t m = std::size(*srcit);
            size_t n = std::distance(srcit, eit);

            for (unsigned int j = 0; j < n; ++j)
            {
                auto &v = *srcit;
                for (unsigned int i = 0; i < m; ++i)
                {
                    tgt(i, j) = v[i];
                }
                srcit++;
            }
        }
        template <class M, class VV> void copy_from_cols(M &tgt, const VV &vecs)
        {
            copy_from_cols(tgt, std::cbegin(vecs), std::cend(vecs));
        }

        template <class A, class B> bool equal(const A &a, const B &b)
        {
            return ralgo::vecops::boolean_all(op_eq(), a, b);
        }

        template <class A, class B, class T>
        bool equal(const A &a, const B &b, T epsilon)
        {
            return ralgo::vecops::boolean_all(op_eq(), a, b, epsilon);
        }

        template <class A, class B> void assign(const A &a, B &b)
        {
            size_t m = a.rows();
            size_t n = a.cols();

            b.resize(m, n);

            for (unsigned int i = 0; i < m; ++i)
                for (unsigned int j = 0; j < n; ++j)
                {
                    b(i, j) = a(i, j);
                }
        }

        template <class A,
                  class B,
                  class C,
                  std::enable_if_t<is_matrix_compatible<B>(), int> = 0>
        void multiply(const A &a, const B &b, C &c)
        {
            int m = a.rows();
            int p = a.cols();
            int n = b.cols();

            c.resize(a.rows(), b.cols());

            for (int i = 0; i < m; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    typename C::value_type acc = 0;
                    for (int k = 0; k < p; ++k)
                    {
                        acc += a.at(i, k) * b.at(k, j);
                    }
                    c.at(i, j) = acc;
                }
            }
        }

        template <class A,
                  class B,
                  class C,
                  std::enable_if_t<is_vector_compatible<B>(), int> = 0>
        void multiply(const A &a, const B &b, C &c)
        {
            int m = a.rows();
            int n = a.cols();

            c.resize(a.rows());

            for (int i = 0; i < m; ++i)
            {
                typename C::value_type acc = 0;
                for (int k = 0; k < n; ++k)
                {
                    acc += a.at(i, k) * b[k];
                }
                c[i] = acc;
            }
        }

        template <class C = void,
                  class A,
                  class B,
                  std::enable_if_t<is_matrix_compatible<B>(), int> = 0>
        auto multiply(const A &a, const B &b)
            -> defsame_t<C,
                         ralgo::matrix<decltype(value_t<A>{} * value_t<B>{})>>
        {
            defsame_t<C, ralgo::matrix<decltype(value_t<A>{} * value_t<B>{})>>
                res;
            multiply(a, b, res);
            return res;
        }

        template <class C = void,
                  class A,
                  class B,
                  std::enable_if_t<is_vector_compatible<B>(), int> = 0>
        auto multiply(const A &a, const B &b)
            -> defsame_t<C,
                         ralgo::vector<decltype(value_t<A>{} * value_t<B>{})>>
        {
            defsame_t<C, ralgo::vector<decltype(value_t<A>{} * value_t<B>{})>>
                res;
            multiply(a, b, res);
            return res;
        }

        template <class A, class B> int square_matrix_inverse(const A &a, B &b)
        {
            // a = input matrix
            // b = result matrix
            // n = number of rows = number of columns in b (n x n)
            int n = a.cols();
            int pivrow;  // keeps track of current pivot row
            int k, i, j; // k: overall index along diagonal; i: row index; j:
                         // col index
            std::vector<int> pivrows(
                n);     // keeps track of rows swaps to undo at end
            double tmp; // used for finding max value and making column swaps

            b.resize(a.cols(), a.cols());

            for (i = 0; i < n; i++)
                for (j = 0; j < n; j++)
                    b.at(i, j) = a.at(i, j);

            for (k = 0; k < n; k++)
            {
                // find pivot row, the row with biggest entry in current column
                tmp = 0;
                for (i = k; i < n; i++)
                {
                    if (abs(b.at(i, k)) >=
                        tmp) // 'Avoid using other functions inside abs()?'
                    {
                        tmp = abs(b.at(i, k));
                        pivrow = i;
                    }
                }

                // check for singular matrix
                if (b.at(pivrow, k) == 0.0f)
                {
                    return -1;
                }

                // Execute pivot (row swap) if needed
                if (pivrow != k)
                {
                    // swap row k with pivrow
                    for (j = 0; j < n; j++)
                    {
                        tmp = b.at(k, j);
                        b.at(k, j) = b.at(pivrow, j);
                        b.at(pivrow, j) = tmp;
                    }
                }
                pivrows[k] =
                    pivrow; // record row swap (even if no swap happened)

                tmp = 1.0f / b.at(k, k); // invert pivot element
                b.at(k, k) =
                    1.0f; // This element of input matrix becomes result matrix

                // Perform row reduction (divide every element by pivot)
                for (j = 0; j < n; j++)
                {
                    b.at(k, j) = b.at(k, j) * tmp;
                }

                // Now eliminate all other entries in this column
                for (i = 0; i < n; i++)
                {
                    if (i != k)
                    {
                        tmp = b.at(i, k);
                        b.at(i, k) = 0.0f; // The other place where in matrix
                                           // becomes result mat
                        for (j = 0; j < n; j++)
                        {
                            b.at(i, j) = b.at(i, j) - b.at(k, j) * tmp;
                        }
                    }
                }
            }

            // Done, now need to undo pivot row swaps by doing column swaps in
            // reverse order
            for (k = n - 1; k >= 0; k--)
            {
                if (pivrows[k] != k)
                {
                    for (i = 0; i < n; i++)
                    {
                        tmp = b.at(i, k);
                        b.at(i, k) = b.at(i, pivrows[k]);
                        b.at(i, pivrows[k]) = tmp;
                    }
                }
            }
            return 0;
        }

        template <class M, class T>
        void diag(M &mat, const igris::array_view<T> &arr)
        {
            mat.resize(arr.size(), arr.size());
            for (int i = 0; i < mat.rows(); ++i)
            {
                for (int j = 0; j < mat.cols(); ++j)
                {
                    mat.at(i, j) = i == j ? arr[i] : 0;
                }
            }
        }

        template <class M> void eye(M &mat)
        {
            for (int i = 0; i < mat.rows(); ++i)
            {
                for (int j = 0; j < mat.cols(); ++j)
                {
                    mat.at(i, j) = i == j ? value_t<M>{1} : 0;
                }
            }
        }

        template <class M, class T>
        void diag(M &mat, const std::initializer_list<T> &arr)
        {
            diag(mat, igris::array_view<T>(arr));
        }

        template <class R = void, class T>
        auto diag(const std::initializer_list<T> &arr)
            -> defsame_t<R, matrix<T>>
        {
            defsame_t<R, matrix<T>> ret;
            diag(ret, arr);
            return ret;
        }

        template <class R = void, class M>
        auto inverse(const M &mat) -> defsame_t<R, matrix<value_t<M>>>
        {
            defsame_t<R, matrix<value_t<M>>> ret;

            square_matrix_inverse(mat, ret);

            return ret;
        }

        template <class M = void>
        defsame_t<M, ralgo::matrix<double>> eye(size_t N)
        {
            defsame_t<M, ralgo::matrix<double>> ret(N, N);
            eye(ret);
            return ret;
        }
    }
}

#endif
