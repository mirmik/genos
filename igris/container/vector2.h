#ifndef IGRIS_VECTOR_H
#define IGRIS_VECTOR_H
/*
#include <algorithm>
#include <cassert>
#include <igris/util/ctrdtr.h>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <utility>
*/

//#include <igris/util/ctrdtr.h>
#include <new>
#include <stdint.h>
#include <stdlib.h>

namespace igris
{
    template <typename T> class vector2
    {
    public:
        using iterator = T *;
        using const_iterator = const T *;
        using value_type = T;
        using reference = T &;
        using const_reference = const T &;
        using size_type = uint32_t;
        using difference_type = int32_t;
        // using iterator_category = std::random_access_iterator_tag;
        using reverse_iterator = T *;
        using const_reverse_iterator = const T *;

    private:
        // resources
        T *m_data = nullptr;
        size_type m_capacity = 0;
        size_type m_size = 0;

        // generated
        // Allocator m_alloc = {};

    public:
        T *data()
        {
            return m_data;
        }
        const T *data() const
        {
            return m_data;
        }
        size_type size() const
        {
            return m_size;
        }
        size_type capacity() const
        {
            return m_capacity;
        }

        vector2() {}

        /*vector(const std::initializer_list<T> &initializers)
        {
            reserve(initializers.size());
            for (auto &a : initializers)
                push_back(a);
        }*/

        /*        vector(std::initializer_list<T> &&initializers)
                {
                    reserve(initializers.size());
                    for (auto &a : initializers)
                        push_back(a);
                }*/

        vector2(const vector2 &other) : m_size(other.m_size)
        {
            m_data = malloc(m_size * sizeof(T));
            m_capacity = m_size;
            for (auto ip = other.m_data, op = m_data;
                 ip != other.m_data + other.m_size;
                 ip++, op++)
            {
                new (op) T(*ip);
            }
        }

        template <class I, class O> vector2(I first, O last)
        {
            reserve(last - first);
            for (; first != last; first++)
            {
                push_back(*first);
            }
        }

        vector2(vector2 &&other)
            : m_data(other.m_data), m_capacity(other.m_capacity),
              m_size(other.m_size)
        {
            other.m_data = nullptr;
            other.m_capacity = 0;
            other.m_size = 0;
        }

        vector2 &operator=(const vector2 &other)
        {
            if (this == &other)
                return *this;

            invalidate();

            m_data = malloc(m_size * sizeof(T));
            m_size = other.m_size;
            m_capacity = m_size;
            for (auto ip = other.m_data, op = m_data;
                 ip != other.m_data + other.m_size;
                 ip++, op++)
            {
                new (op) T(*ip);
            }

            return *this;
        }

        vector2 &operator=(vector2 &&other)
        {
            if (this == &other)
                return *this;
            m_data = other.m_data;
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            other.m_data = nullptr;
            other.m_capacity = 0;
            other.m_size = 0;
            return *this;
        }

        vector2(size_type sz) : m_data(nullptr), m_capacity(0), m_size(0)
        {
            resize(sz);
        }

        vector2(iterator a, const iterator b)
            : m_data(nullptr), m_capacity(0), m_size(0)
        {
            while (a != b)
            {
                push_back(*a++);
            }
        }

        ~vector2()
        {
            invalidate();
        }

        T &front()
        {
            return m_data[0];
        }

        T &back()
        {
            return m_data[m_size - 1];
        }

        void invalidate()
        {
            if (m_data)
            {
                for (auto it = begin(); it != end(); it++)
                    (*it).~T();
                if (m_data)
                    free(m_data);
            }
            m_data = nullptr;
            m_size = 0;
            m_capacity = 0;
        }

        unsigned char reserve(size_type sz)
        {
            if (sz > m_capacity)
                return changeBuffer(sz);
            return 1;
        }

        void clear()
        {
            for (unsigned int i = 0; i < m_size; ++i)
            {
                (m_data + i)->~T();
            }
            m_size = 0;
        }

        iterator begin()
        {
            return m_data;
        }
        iterator end()
        {
            return m_data + m_size;
        }
        iterator rbegin()
        {
            return m_data + m_size - 1;
        }
        const_iterator rend()
        {
            return m_data - 1;
        }

        const_iterator begin() const
        {
            return m_data;
        }
        const_iterator end() const
        {
            return m_data + m_size;
        }
        const_iterator rbegin() const
        {
            return m_data + m_size - 1;
        }
        const_iterator rend() const
        {
            return m_data - 1;
        }

        /*template <typename... Args> void emplace_back(Args &&... args)
        {
            reserve(m_size + 1);
            new (m_data + m_size) T(std::forward<Args>(args)...);
            m_size++;
        }*/

        void push_back(const T &ref)
        {
            reserve(m_size + 1);
            new (m_data + m_size) T(ref);
            m_size++;
        }

        void pop_back()
        {
            (m_data + m_size - 1)->~T();
            m_size--;
        }

        bool empty() const
        {
            return m_size == 0;
        }

        bool operator==(const vector2 &oth) const
        {
            if (size() != oth.size())
                return false;

            auto it = begin();
            auto eit = end();
            auto bit = oth.begin();

            for (; it != eit; ++it, ++bit)
            {
                if (*it != *bit)
                    return false;
            }

            return true;
        }

        bool operator!=(const vector2 &oth) const
        {
            return !operator==(oth);
        }

        /*template <typename... Args>
        iterator emplace(const_iterator pos, Args &&... args)
        {
            // TODO insert optimization
            size_type _pos = pos - m_data;

            reserve(m_size + 1);
            m_size++;

            iterator first = m_data + _pos;
            iterator last = std::prev((iterator)end());
            std::move_backward(first, last, end());
            new (first) T(std::forward<Args>(args)...);

            return first;
        }

        iterator insert(const_iterator pos, const T &value)
        {
            // TODO insert optimization
            size_type _pos = pos - m_data;

            reserve(m_size + 1);
            m_size++;

            iterator first = m_data + _pos;
            iterator last = std::prev((iterator)end());
            std::move_backward(first, last, (iterator)end());
            *first = value;

            return first;
        }

        iterator insert(iterator pos, const_iterator first, const_iterator last)
        {
            size_type _pos = pos - m_data;
            size_type _first = first - m_data;
            size_type _last = last - m_data;

            size_type sz = _last - _first;
            reserve(m_size + sz);
            m_size += sz;

            iterator first_it = m_data + _pos;
            iterator last_it = std::prev((iterator)end(), sz);
            std::move_backward(first_it, last_it, (iterator)end());
            std::copy(m_data + _first, m_data + _last, first_it);

            return first_it;
        }

        iterator insert(int pos, const T &value)
        {
            return insert(m_data + pos, value);
        }

        iterator insert_sorted(T const &item)
        {
            return insert(std::upper_bound(begin(), (iterator)end(), item),
                          item);
        }*/

        void resize(size_type n)
        {
            reserve(n);
            size_type oldsize = m_size;
            if (n > oldsize)
            {
                for (size_type i = oldsize; i < n; ++i)
                {
                    new (m_data + i) T();
                }
            }
            else
            {
                for (size_type i = n; i < oldsize; ++i)
                {
                    new (m_data + i) T();
                }
            }
            m_size = n;
        }

        void erase(iterator newend)
        {
            m_size = newend - m_data;
        }

        /*void erase(iterator first, iterator last)
        {
            size_type sz = last - first;
            for (size_type i = 0; i < sz; ++i)
            {
                (first + i)->~T();
            }
            std::move(last, end(), first);
            m_size -= sz;
        }*/

        /* T &at(size_type num)
         {
             if (num >= m_size)
                 throw std::out_of_range("vector::at");
             return m_data[num];
         }*/

        T &operator[](size_type num)
        {
            return m_data[num];
        }

        const T &operator[](size_type num) const
        {
            return m_data[num];
        }

        /*const T &at(size_type num) const
        {
            if (num >= m_size)
                throw std::out_of_range("vector::at");
            return m_data[num];
        }*/

        const T &back() const
        {
            return m_data[m_size - 1];
        }

        const T &front() const
        {
            return m_data[0];
        }

        /*bool operator<(const vector &oth) const
        {
            return std::lexicographical_compare(
                begin(), end(), oth.begin(), oth.end());
        }*/

    protected:
        unsigned char changeBuffer(size_type sz)
        {
            // size_type oldcapacity = m_capacity;
            auto newbuf = (T *)(malloc(sz * sizeof(T)));
            m_capacity = sz;
            if (m_data == nullptr)
            {
                m_data = newbuf;
                return 1;
            }

            auto ie = end();
            auto op = newbuf;
            for (auto ip = begin(); ip != ie; op++, ip++)
            {
                // igris::move_constructor(op, std::move(*ip));
                *op = (T &&) * ip;
            }

            {
                auto it = begin();
                auto eit = end();
                for (; it != eit; it++)
                {
                    (it)->~T();
                }
            }

            // igris::array_destructor(begin(), end());
            auto oldbuf = m_data;
            m_data = newbuf;
            free(oldbuf);
            return 1;
        }
    };
}

#endif