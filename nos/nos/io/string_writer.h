#ifndef NOS_IO_STRING_WRITER_H
#define NOS_IO_STRING_WRITER_H

#include <nos/io/ostream.h>
#include <string>

namespace nos
{
    class string_writer : public nos::ostream
    {
    private:
        std::string &str;

    public:
        string_writer(std::string &_str) : str(_str) {}

        nos::expected<size_t, nos::output_error> write(const void *ptr,
                                                       size_t sz) override
        {
            str.append((char *)ptr, sz);
            return sz;
        }
    };

    class string_buffer : public nos::ostream
    {
    private:
        std::string _str = {};

    public:
        nos::expected<size_t, nos::output_error> write(const void *ptr,
                                                       size_t sz) override
        {
            _str.append((char *)ptr, sz);
            return sz;
        }

        std::string &str()
        {
            return _str;
        }
    };

    class cstring_writer : public nos::ostream
    {
    private:
        char *str;

    public:
        cstring_writer(char *_str) : str(_str) {}
        cstring_writer(const cstring_writer &_str) = default;
        cstring_writer &operator=(const cstring_writer &_str) = default;

        nos::expected<size_t, nos::output_error> write(const void *ptr,
                                                       size_t sz) override
        {
            memcpy(str, ptr, sz);
            str += sz;
            return (int)sz;
        }
    };
    /*
    class buffer_writer : public nos::ostream
    {
    private:
        char *str;
        size_t len;

    public:
        buffer_writer(char *_str, int _room) : str(_str), len(_room) {}
        buffer_writer(char *_str, size_t _room) : str(_str), len(_room) {}
        buffer_writer(const buffer_writer &_str) = default;
        buffer_writer &operator=(const buffer_writer &) = default;

        int write(const void *ptr, size_t sz) override
        {
            int l = sz < len ? sz : len;
            memcpy(str, ptr, l);
            str += l;
            len -= l;
            return l;
        }
    };*/
}

#endif
