#ifndef RALGO_VECTOR_VIEW_H
#define RALGO_VECTOR_VIEW_H

#include <cstddef>
#include <iostream>
#include <memory>

/**
    @file
*/

namespace ralgo
{
    template <class T> class vector_view_iterator
    {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;
        using stride_type = int;

    private:
        pointer dat;
        stride_type stride;

    public:
        vector_view_iterator(T *dat) : dat(dat), stride(1) {}
        vector_view_iterator(T *dat, int stride) : dat(dat), stride(stride) {}

        std::ptrdiff_t operator-(const vector_view_iterator &oth)
        {
            return (dat - oth.dat) / stride;
        }

        vector_view_iterator &operator++()
        {
            dat += stride;
            return *this;
        }

        vector_view_iterator operator++(int)
        {
            vector_view_iterator ret = *this;
            dat += stride;
            return ret;
        }

        T &operator*()
        {
            return *dat;
        }
        T &operator->()
        {
            return *dat;
        }

        bool operator!=(const vector_view_iterator &oth) const
        {
            return dat != oth.dat || stride != oth.stride;
        }

        bool operator==(const vector_view_iterator &oth) const
        {
            return dat == oth.dat && stride == oth.stride;
        }
    };

    template <class T> class vector_view
    {
        T *dat;
        int n;
        int stride;

    public:
        using iterator = vector_view_iterator<T>;
        using value_type = T;

        void resize(int sz)
        {
            n = sz;
        }

        vector_view() : dat(nullptr), n(0), stride(0) {}
        vector_view(T *dat, int n) : dat(dat), n(n), stride(1) {}
        vector_view(T *dat, size_t n) : dat(dat), n((int)n), stride(1) {}
        vector_view(T *dat, int n, int stride) : dat(dat), n(n), stride(stride)
        {
        }

        template <class U>
        vector_view(const U &vec) : dat(vec), n(vec.size()), stride(1)
        {
        }

        T &operator[](int i)
        {
            return *(dat + i * stride);
        }
        const T &operator[](int i) const
        {
            return *(dat + i * stride);
        }

        T *data()
        {
            return dat;
        }
        const T *data() const
        {
            return dat;
        }
        size_t size() const
        {
            return n;
        }

        iterator begin()
        {
            return {dat, stride};
        }
        iterator end()
        {
            return {dat + n * stride, stride};
        }
        const iterator begin() const
        {
            return {dat, stride};
        }
        const iterator end() const
        {
            return {dat + n * stride, stride};
        }
    };

    template <class T>
    std::ostream &operator<<(std::ostream &os, const vector_view<T> &vec)
    {
        os << '(';
        for (unsigned int i = 0; i < vec.size(); ++i)
        {
            os << vec[i] << ',';
        }
        os << vec[vec.size() - 1] << ')';
        return os;
    }
}

#endif
