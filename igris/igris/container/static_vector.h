#ifndef IGRIS_CONTAINER_STATIC_VECTOR_H
#define IGRIS_CONTAINER_STATIC_VECTOR_H

#include <algorithm>
#include <cstring>
#include <igris/util/ctrdtr.h>
#include <initializer_list>
#include <new>
#include <type_traits>
#include <utility>

namespace igris
{
    template <class T, std::size_t N> class static_vector
    {
    public:
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using iterator = T *;
        using const_iterator = const T *;

    private:
        // Переместить во внешний буффер.
        typename std::aligned_storage<sizeof(T), alignof(T)>::type _data[N];
        std::size_t m_size = 0;

    public:
        static_vector()
        {
            memset(_data, 0, sizeof(_data));
        }

        static_vector(const static_vector &other)
        {
            m_size = other.m_size;
            for (std::size_t pos = 0; pos < m_size; ++pos)
            {
                new (&_data[pos]) T(other[pos]);
            }
        }

        static_vector(static_vector &&other)
        {
            m_size = other.m_size;
            for (std::size_t pos = 0; pos < m_size; ++pos)
            {
                new (&_data[pos]) T(std::move(other[pos]));
            }
            other.clear();
        }

        static_vector &operator=(const static_vector &other)
        {
            m_size = other.m_size;
            for (std::size_t pos = 0; pos < m_size; ++pos)
            {
                new (&_data[pos]) T(other[pos]);
            }
            return *this;
        }

        static_vector &operator=(static_vector &&other)
        {
            m_size = other.m_size;
            for (std::size_t pos = 0; pos < m_size; ++pos)
            {
                new (&_data[pos]) T(std::move(other[pos]));
            }
            other.clear();
            return *this;
        }

        template <class It> static_vector(It b, It e)
        {
            for (; b != e; ++b)
            {
                push_back(*b);
            }
        }

        static_vector(const std::initializer_list<T> &lst)
        {
            for (auto &obj : lst)
            {
                new (&_data[m_size]) T(obj);
                ++m_size;
            }
        }

        // Create an object in aligned storage
        template <typename... Args> void emplace_back(Args &&... args)
        {
            if (m_size >= N)
                return;
            new (&_data[m_size]) T(std::forward<Args>(args)...);
            ++m_size;
        }

        void push_back(const T &obj)
        {
            if (m_size >= N)
                return;
            new (&_data[m_size]) T(obj);
            ++m_size;
        }

        T &operator[](std::size_t pos)
        {
            return *reinterpret_cast<T *>(&_data[pos]);
        }

        const T &operator[](std::size_t pos) const
        {
            return *reinterpret_cast<const T *>(&_data[pos]);
        }

        T *data()
        {
            return reinterpret_cast<T *>(&_data[0]);
        }

        const T *data() const
        {
            return reinterpret_cast<const T *>(&_data[0]);
        }

        std::size_t room() const
        {
            return N - m_size;
        }
        std::size_t size() const
        {
            return m_size;
        }

        // Delete objects from aligned storage
        ~static_vector()
        {
            for (std::size_t pos = 0; pos < m_size; ++pos)
            {
                reinterpret_cast<T *>(&_data[pos])->~T();
            }
            m_size = 0;
        }

        iterator begin()
        {
            return reinterpret_cast<T *>(&_data[0]);
        }

        iterator end()
        {
            return reinterpret_cast<T *>(&_data[m_size]);
        }

        const_iterator begin() const
        {
            return reinterpret_cast<const T *>(&_data[0]);
        }

        const_iterator end() const
        {
            return reinterpret_cast<const T *>(&_data[m_size]);
        }

        T &back()
        {
            return *reinterpret_cast<T *>(&_data[m_size - 1]);
        }

        const T &back() const
        {
            return *reinterpret_cast<const T *>(&_data[m_size - 1]);
        }

        T &front()
        {
            return *reinterpret_cast<T *>(&_data[0]);
        }

        const T &front() const
        {
            return *reinterpret_cast<const T *>(&_data[0]);
        }

        void erase(iterator first, iterator last)
        {
            size_t sz = last - first;
            for (size_t i = 0; i < sz; ++i)
            {
                igris::destructor(first + i);
            }
            std::move(last, end(), first);
            m_size -= sz;
        }

        void resize(size_t newsize)
        {
            if (newsize >= N)
                newsize = N;

            for (size_t i = m_size; i < newsize; ++i)
            {
                new (&_data[i]) T{};
            }

            m_size = newsize;
        }

        void clear()
        {
            m_size = 0;
        }
    };
}

#endif
