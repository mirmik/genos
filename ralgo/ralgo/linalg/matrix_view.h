#ifndef RALGO_LINALG_MATRIX_VIEW_H
#define RALGO_LINALG_MATRIX_VIEW_H

/**
    Соглашение об интерфейсе матриц в библиотеке ralgo
    взято от матриц boost::matrix.

    Основные операции -
    Геттер - matrix(i,j)
    Количество строк - matrix.size1()
    Количество столбцов - matrix.size2()
*/

#include <initializer_list>
#include <iostream>
#include <ralgo/linalg/vector_view.h>

namespace ralgo
{
    template <class T> struct row_order
    {
        using value_type = T;
        static T &at(T *data, int i, int j, int stride)
        {
            return *(data + i * stride + j);
        }
        static int stride(int rows, int cols)
        {
            (void)rows;
            return cols;
        }
        static vector_view<T> sect(T *data, int i, int rows, int cols)
        {
            (void)rows;
            return {data + i * cols, cols, 1};
        }
        static vector_view<T>
        row(T *data, int i, int rows, int cols, int stride)
        {
            (void)rows;
            (void)stride;
            return {data + i * cols, cols, 1};
        }
        static vector_view<T>
        col(T *data, int i, int rows, int cols, int stride)
        {
            (void)cols;
            return {data + i, rows, stride};
        }
    };

    template <class T> struct collumn_order
    {
        using value_type = T;
        static T &at(T *data, int i, int j, int stride)
        {
            return *(data + j * stride + i);
        }
        static int stride(int rows, int cols)
        {
            (void)cols;
            return rows;
        }
        static vector_view<T> sect(T *data, int i, int rows, int cols)
        {
            (void)cols;
            return {data + i * rows, rows, 1};
        }
        static vector_view<T>
        row(T *data, int i, int rows, int cols, int stride)
        {
            (void)cols;
            (void)stride;
            return {data + i * rows, rows, 1};
        }
        static vector_view<T>
        col(T *data, int i, int rows, int cols, int stride)
        {
            (void)rows;
            return {data + i, cols, stride};
        }
    };

    template <class T, class O = row_order<T>> class matrix_view
    {
    protected:
        T *_data = nullptr;
        int _rows = 0;
        int _cols = 0;
        int _stride = 0;

        O accessor = {};

    public:
        using __check_O_validity =
            typename O::value_type; // order type validity
        using value_type = T;

        matrix_view() : _data(nullptr), _rows(0), _cols(0), _stride(0) {}

        matrix_view(T *data, int rows, int cols, int stride)
            : _data(data), _rows(rows), _cols(cols), _stride(stride)
        {
        }

        matrix_view(T *data, int rows, int cols)
            : _data(data), _rows(rows), _cols(cols),
              _stride(accessor.stride(rows, cols))
        {
        }

        matrix_view(const matrix_view &oth) = default;
        matrix_view &operator=(const matrix_view &oth) = default;

        matrix_view(const std::initializer_list<T> &lst, int rows, int cols)
            : matrix_view((T *)std::begin(lst), rows, cols)
        {
        }

        int rows() const
        {
            return _rows;
        }
        int cols() const
        {
            return _cols;
        }
        int size() const
        {
            return _rows * _cols;
        }

        size_t size1() const
        {
            return _rows;
        }
        size_t size2() const
        {
            return _cols;
        }

        T *data()
        {
            return _data;
        }
        const T *data() const
        {
            return _data;
        }

        void resize(int rows, int cols, int stride = 0)
        {
            _rows = rows;
            _cols = cols;
            if (stride == 0)
                _stride = accessor.stride(rows, cols);
            else
                _stride = stride;
        }

        T &at(int i, int j)
        {
            return accessor.at(_data, i, j, _stride);
        }
        const T &at(int i, int j) const
        {
            return accessor.at(_data, i, j, _stride);
        }

        T &operator()(int i, int j)
        {
            return at(i, j);
        }
        const T &operator()(int i, int j) const
        {
            return at(i, j);
        }

        ralgo::vector_view<T> operator[](int i)
        {
            return accessor.sect(_data, i, _rows, _cols);
        }
        const ralgo::vector_view<T> operator[](int i) const
        {
            return accessor.sect(_data, i, _rows, _cols);
        }

        ralgo::vector_view<T> row(int i)
        {
            return accessor.row(_data, i, _rows, _cols, _stride);
        }
        ralgo::vector_view<T> col(int i)
        {
            return accessor.col(_data, i, _rows, _cols, _stride);
        }
        const ralgo::vector_view<T> row(int i) const
        {
            return accessor.row(_data, i, _rows, _cols, _stride);
        }
        const ralgo::vector_view<T> col(int i) const
        {
            return accessor.col(_data, i, _rows, _cols, _stride);
        }

        // vecops compatible. Убрать, если потребуются смещения.
        T *begin()
        {
            return _data;
        }
        T *end()
        {
            return _data + _cols * _rows;
        } // Stride ?
        const T *begin() const
        {
            return _data;
        }
        const T *end() const
        {
            return _data + _rows * _cols;
        } // Stride ?

        void release()
        {
            _data = nullptr;
            _rows = 0;
            _cols = 0;
            _stride = 0;
        }

        bool operator==(const matrix_view &oth) const
        {
            if (rows() != oth.rows())
                return false;
            if (cols() != oth.cols())
                return false;

            for (int i = 0; i < oth.rows(); ++i)
                for (int j = 0; j < oth.cols(); ++j)
                    if (at(i, j) != oth.at(i, j))
                        return false;

            return true;
        }

        bool operator!=(const matrix_view &oth) const
        {
            return !operator==(oth);
        }

        template <class M> bool operator==(const M &oth) const
        {
            if (rows() != oth.rows())
                return false;
            if (cols() != oth.cols())
                return false;

            for (int i = 0; i < oth.rows(); ++i)
                for (int j = 0; j < oth.cols(); ++j)
                    if (at(i, j) != oth.at(i, j))
                        return false;

            return true;
        }

        template <class M> bool operator!=(const M &oth) const
        {
            return !operator==(oth);
        }
    };

    template <class T> using matrix_view_co = matrix_view<T, collumn_order<T>>;
    template <class T> using matrix_view_ro = matrix_view<T, row_order<T>>;

    template <class T, class O>
    std::ostream &operator<<(std::ostream &os, const matrix_view<T, O> &m)
    {
        if (m.rows() == 0 || m.cols() == 0)
        {
            os << "(null_matrix)";
            return os;
        }

        for (int i = 0; i < m.rows(); ++i)
        {
            for (int j = 0; j < m.cols(); ++j)
            {
                os << m.at(i, j) << " ";
            }
            os << std::endl;
        }

        return os;
    }
}

#endif
