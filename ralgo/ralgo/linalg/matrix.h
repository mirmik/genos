#ifndef RALGO_LINALG_MATRIX_H
#define RALGO_LINALG_MATRIX_H

/**
@file
*/

#include <memory>
#include <ralgo/linalg/matrix_view.h>

namespace ralgo
{
    template <class T,
              class O = ralgo::row_order<T>,
              class Alloc = std::allocator<T>>
    class matrix : public matrix_view<T, O>
    {
        using parent = matrix_view<T, O>;
        Alloc alloc = {};

    public:
        matrix() : parent() {}

        matrix(const std::initializer_list<const std::initializer_list<T>> &lst)
        {
            T *iter;

            resize(lst.size(), lst.begin()->size());
            iter = parent::_data;

            for (auto a : lst)
            {
                for (auto v : a)
                {
                    *iter++ = v;
                }
            }
        }

        matrix(const matrix &oth) : parent()
        {
            int rows = oth.rows();
            int cols = oth.cols();

            resize(rows, cols);
            for (int i = 0; i < rows; ++i)
                for (int j = 0; j < cols; ++j)
                    this->at(i, j) = oth.at(i, j);
        }

        matrix(int r, int c) : parent(nullptr, r, c)
        {
            this->_data = alloc.allocate(r * c);
        }

        void resize(int r, int c)
        {
            invalidate();

            this->_data = alloc.allocate(r * c);
            parent::resize(r, c);
        }

        ~matrix()
        {
            invalidate();
        }

        void invalidate()
        {
            if (this->_data)
            {
                alloc.deallocate(this->_data, this->_rows * this->_cols);
            }

            parent::release();
        }

        matrix &operator=(const matrix &oth)
        {
            if (this == &oth)
                return *this;

            int rows = oth.rows();
            int cols = oth.cols();

            resize(rows, cols);
            for (int i = 0; i < rows; ++i)
                for (int j = 0; j < cols; ++j)
                    this->at(i, j) = oth.at(i, j);

            return *this;
        }

        template <class M> matrix &operator=(const M &oth)
        {
            if (this == &oth)
                return *this;

            int rows = oth.rows();
            int cols = oth.cols();

            resize(rows, cols);
            for (int i = 0; i < rows; ++i)
                for (int j = 0; j < cols; ++j)
                    this->at(i, j) = oth.at(i, j);

            return *this;
        }
    };
}

#endif
