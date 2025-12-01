#ifndef IGRIS_UNBOUNDED_ARRAY_H
#define IGRIS_UNBOUNDED_ARRAY_H

#include <igris/container/array_view.h>
#include <memory>

namespace igris
{
    template <typename T, typename A = std::allocator<T>> class unbounded_array
    {
    public:
        using allocator_type = A;
        using value_type = T;
        using reference = T &;
        using const_reference = const T &;

        A alloc = {};
        T *m_data = nullptr;
        size_t m_size = 0;

        T *data()
        {
            return m_data;
        }
        const T *data() const
        {
            return m_data;
        }
        size_t size() const
        {
            return m_size;
        }

        unbounded_array() : m_data(nullptr), m_size(0) {}

        unbounded_array(size_t sz)
            : alloc{}, m_data(alloc.allocate(sz)), m_size(sz)
        {
            for (size_t i = 0; i < sz; ++i)
            {
                new (m_data + i) T();
            }
        }

        unbounded_array(const T *data, size_t sz) : unbounded_array(sz)
        {
            std::copy(data, data + sz, m_data);
        }

        unbounded_array(const igris::array_view<T> &buf)
            : unbounded_array(buf.data(), buf.size())
        {
        }

        unbounded_array(const std::initializer_list<T> &buf)
            : unbounded_array(buf.begin(), buf.size())
        {
        }

        unbounded_array(const unbounded_array &oth)
            : unbounded_array(oth.data(), oth.size())
        {
        }

        void clear()
        {
            invalidate();
        }

        void fill(const T &val)
        {
            for (auto &ref : *this)
            {
                ref = val;
            }
        }

        unbounded_array &operator=(const unbounded_array &oth)
        {
            m_data = alloc.allocate(oth.size());
            m_size = oth.size();

            auto ptr = m_data;
            for (const auto &ref : oth)
            {
                new (ptr++) T(ref);
            }

            return *this;
        }

        unbounded_array(unbounded_array &&arr)
            : m_data(arr.m_data), m_size(arr.m_size), alloc(arr.alloc)
        {
            arr.m_size = 0;
            arr.m_data = nullptr;
        }

        void resize(size_t size)
        {
            invalidate();
            create_buffer(size);
        }

        void create_buffer(size_t size)
        {
            m_data = alloc.allocate(size);
            m_size = size;
        }

        void invalidate()
        {
            for (size_t i = 0; i < m_size; ++i)
            {
                m_data[i].~T();
            }
            alloc.deallocate(m_data, m_size);
            m_data = nullptr;
            m_size = 0;
        }

        ~unbounded_array()
        {
            invalidate();
        }

        T &operator[](size_t i)
        {
            return *(m_data + i);
        }

        const T &operator[](size_t i) const
        {
            return *(m_data + i);
        }

        using iterator = T *;
        using const_iterator = const T *;

        iterator begin()
        {
            return m_data;
        }
        iterator end()
        {
            return m_data + m_size;
        }

        const_iterator begin() const
        {
            return (const_iterator)m_data;
        }
        const_iterator end() const
        {
            return (const_iterator)m_data + m_size;
        }
    };
}

#endif
