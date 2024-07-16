#ifndef IGRIS_CONTAINER_STATIC_STRING_H
#define IGRIS_CONTAINER_STATIC_STRING_H

#include <cstring>
#include <new>
#include <type_traits>
#include <utility>

namespace igris
{
    template <std::size_t N> class static_string
    {
        using pointer = char *;
        using reference = char &;
        using iterator = char *;
        using const_iterator = const char *;

        // Переместить во внешний буффер.
        mutable char data[N + 1];
        std::size_t m_size = 0;

    public:
        static_string() = default;

        static_string(const char *dat)
        {
            m_size = strlen(dat);
            memcpy(data, dat, m_size);
        }

        char &operator[](std::size_t pos)
        {
            return &data[pos];
        }
        const char &operator[](std::size_t pos) const
        {
            return &data[pos];
        }

        std::size_t room()
        {
            return N - m_size;
        }
        std::size_t size()
        {
            return m_size;
        }

        // Delete objects from aligned storage
        ~static_string() = default;

        iterator begin()
        {
            return &data[0];
        }
        const_iterator end()
        {
            return &data[m_size];
        }

        void push_back(char c)
        {
            if (m_size >= N)
                return;

            data[m_size++] = c;
        }

        const char *c_str() const
        {
            data[m_size] = 0;
            return data;
        }
    };
}

#endif
