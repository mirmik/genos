/**
    @file
*/

#ifndef IGRIS_BUFFER_H_
#define IGRIS_BUFFER_H_

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>

namespace igris
{
    class buffer
    {
    protected:
        char *buf;
        size_t sz;

    public:
        const char *data() const
        {
            return buf;
        }
        char *data()
        {
            return buf;
        }

        const uint8_t *bytes() const
        {
            return (const uint8_t *)buf;
        }
        uint8_t *bytes()
        {
            return (uint8_t *)buf;
        }

        size_t size() const
        {
            return sz;
        }

        void data(char *buf)
        {
            this->buf = buf;
        }
        void size(size_t sz)
        {
            this->sz = sz;
        }

        buffer &operator=(const buffer &oth)
        {
            buf = oth.buf;
            sz = oth.sz;
            return *this;
        }

        // ctors:
        buffer() : buf(nullptr), sz(0) {}
        buffer(const char *_buf) : buf((char *)_buf), sz(strlen(_buf)) {}

        buffer(const void *_buf, int _sz) : buf((char *)_buf), sz((size_t)_sz)
        {
        }
        buffer(const void *_buf, size_t _sz) : buf((char *)_buf), sz(_sz) {}

        buffer(const igris::buffer &str) : buffer(str.data(), str.size()) {}

        buffer(const std::string &str) : buffer(str.data(), str.size()) {}
        buffer(const std::string_view &str) : buffer(str.data(), str.size()) {}

        template <size_t N>
        inline buffer(const char (&arr)[N]) : buf((char *)arr), sz(N)
        {
        }

        explicit operator std::string()
        {
            return std::string(buf, sz);
        }

        // methods:
        bool operator==(const buffer &other) const
        {
            return (sz == other.sz) &&
                   (strncmp(buf, other.buf, sz < other.sz ? sz : other.sz) ==
                    0);
        }

        bool operator!=(const buffer &other) const
        {
            return (sz != other.sz) ||
                   (strncmp(buf, other.buf, sz < other.sz ? sz : other.sz) !=
                    0);
        }

        bool operator==(const char *str)
        {
            return strncmp(buf, str, sz) == 0;
        }

        bool operator!=(const char *str)
        {
            return strncmp(buf, str, sz) != 0;
        }

        char &operator[](size_t num)
        {
            return *(buf + num);
        }

        char operator[](size_t num) const
        {
            return *(buf + num);
        }

        char *operator*()
        {
            return buf;
        }

        size_t operator+()
        {
            return sz;
        }

        char *begin()
        {
            return buf;
        }

        const char *end()
        {
            return buf + sz;
        }

        bool empty()
        {
            return buf == nullptr;
        }

        buffer slice(size_t idx, size_t _sz)
        {
            return buffer(buf + idx, _sz);
        }

        template <typename T> static igris::buffer on_object(T &obj)
        {
            return buffer((char *)&obj, sizeof(obj));
        }

        std::string to_string()
        {
            return {data(), size()};
        }
    };

    namespace buffer_literal
    {
        inline buffer operator"" _b(const char *str, size_t sz)
        {
            return buffer(str, sz);
        }
    }
}

#endif // IGRIS_BUFFER_H_
