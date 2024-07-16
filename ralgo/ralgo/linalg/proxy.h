#ifndef RALGO_LINALG_TRANSPOSED_MATRIX_VIEW_H
#define RALGO_LINALG_TRANSPOSED_MATRIX_VIEW_H

#include <vector>

/**
 *  Матрицы-прокси. Оболочки, имеющие интерфейс матрицы, но обращающиеся за
 *данными к внешним источникам, или совершающие над данными предварительные
 *операции.
 **/

namespace ralgo
{
    template <class M> class transposed_matrix_proxy
    {
        M &mat;

    public:
        transposed_matrix_proxy(M &mat) : mat(mat) {}

        auto &at(int i, int j)
        {
            return mat.at(j, i);
        }

        const auto &at(int i, int j) const
        {
            return mat.at(j, i);
        }

        int rows() const
        {
            return mat.cols();
        }
        int cols() const
        {
            return mat.rows();
        }

        // ralgo::vector_view<typename M::value_type>
        auto row(int i)
        {
            return mat.col(i);
        }
        auto col(int i)
        {
            return mat.row(i);
        }
    };

    template <class V> class inverted_diagonal_proxy
    {
        V &vec;

    public:
        inverted_diagonal_proxy(V &vec) : vec(vec) {}

        auto at(int i, int j) const
        {
            return i == j ? 1. / vec[i] : 0;
        }

        int rows() const
        {
            return vec.size();
        }
        int cols() const
        {
            return vec.size();
        }
    };

    template <class V> class columns_proxy_matrix
    {
        std::vector<std::reference_wrapper<V>> vecs;

    public:
        columns_proxy_matrix(std::vector<std::reference_wrapper<V>> &vecs)
            : vecs(vecs)
        {
        }

        auto at(int i, int j) const
        {
            return vecs[j][i];
        }
        int rows() const
        {
            return vecs[0].size();
        }
        int cols() const
        {
            return vecs.size();
        }
    };

    template <class V> class rows_proxy_matrix
    {
        std::vector<std::reference_wrapper<V>> vecs;

    public:
        rows_proxy_matrix(std::vector<std::reference_wrapper<V>> &vecs)
            : vecs(vecs)
        {
        }

        auto at(int i, int j) const
        {
            return vecs[i][j];
        }
        int rows() const
        {
            return vecs.size();
        }
        int cols() const
        {
            return vecs[0].size();
        }
    };
}

#endif
