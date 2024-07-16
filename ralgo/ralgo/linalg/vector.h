#ifndef RALGO_LINALG_VECTOR_H
#define RALGO_LINALG_VECTOR_H

#include <initializer_list>
#include <iostream>
#include <ralgo/linalg/vector_view.h>
#include <vector>

namespace ralgo
{
    template <class T, class Alloc = std::allocator<T>> class vector
    {
        using Parent = std::vector<T, Alloc>;
        Parent vec = {};

    public:
        using value_type = typename Parent::value_type;

        vector() = default;
        explicit vector(size_t n, T init = T()) : vec(n)
        {
            for (size_t i = 0; i < n; ++i)
                vec[i] = init;
        }
        vector(T *ptr, size_t n) : vec(ptr, ptr + n) {}
        vector(const T *ptr, size_t n) : vec((T *)ptr, ptr + n) {}
        vector(const std::initializer_list<T> &list) : vec(list) {}
        vector(const vector &oth) = default;
        vector(vector &&oth) = default;

        operator vector_view<T>()
        {
            return vector_view<T>(data(), size());
        }

        auto size() const
        {
            return vec.size();
        }
        auto resize(int arg)
        {
            return vec.resize(arg);
        }
        const value_type *data() const
        {
            return vec.data();
        }
        value_type *data()
        {
            return vec.data();
        }

        void push_back(const T &val)
        {
            vec.push_back(val);
        }

        const T &operator[](int idx) const
        {
            return vec[idx];
        }
        T &operator[](int idx)
        {
            return vec[idx];
        }

        auto begin() const
        {
            return vec.begin();
        }
        auto begin()
        {
            return vec.begin();
        }
        auto end() const
        {
            return vec.end();
        }
        auto end()
        {
            return vec.end();
        }

        vector operator-(const vector &oth) const
        {
            vector ret(this->size());
            for (size_t i = 0; i < this->size(); ++i)
                ret[i] = vec[i] - oth[i];
            return ret;
        }

        vector operator+(const vector &oth) const
        {
            vector ret(this->size());
            for (size_t i = 0; i < this->size(); ++i)
                ret[i] = vec[i] + oth[i];
            return ret;
        }

        vector operator*(double scalar) const
        {
            vector ret(this->size());
            for (size_t i = 0; i < this->size(); ++i)
                ret[i] = vec[i] * scalar;
            return ret;
        }

        vector operator/(double scalar) const
        {
            vector ret(this->size());
            for (size_t i = 0; i < this->size(); ++i)
                ret[i] = vec[i] / scalar;
            return ret;
        }

        bool operator==(const vector &oth) const
        {
            return vec == oth.vec;
        }

        bool operator!=(const vector &oth) const
        {
            return vec != oth.vec;
        }

        bool operator==(const std::vector<T> &oth) const
        {
            return vec == oth;
        }

        bool operator!=(const std::vector<T> &oth) const
        {
            return vec != oth;
        }

        vector &operator=(const std::vector<T> &oth)
        {
            vec = oth;
            return *this;
        }
        vector &operator=(const vector &oth) = default;
        vector &operator=(std::vector<T> &&oth)
        {
            vec = std::move(oth);
            return *this;
        }
        vector &operator=(vector &&oth) = default;

        vector &operator+=(const vector &vec)
        {
            for (size_t i = 0; i < this->size(); ++i)
                (*this)[i] += vec[i];
            return *this;
        }

        vector &operator-=(const vector &vec)
        {
            for (size_t i = 0; i < this->size(); ++i)
                (*this)[i] -= vec[i];
            return *this;
        }
    };
}

namespace std
{
    template <class T>
    std::ostream &operator<<(std::ostream &os, const ralgo::vector<T> &vec)
    {
        if (vec.size() == 0)
        {
            os << "{}";
            return os;
        }

        os << '{';
        for (unsigned int i = 0; i < vec.size() - 1; ++i)
        {
            os << vec[i] << ',';
        }
        if (vec.size() != 0)
            os << vec[vec.size() - 1];
        os << '}';
        return os;
    }
}

#endif
