#ifndef IGRIS_CONTAINER_SLINE_H
#define IGRIS_CONTAINER_SLINE_H

#include <igris/container/unbounded_array.h>
#include <igris/datastruct/sline.h>

namespace igris
{
    class sline
    {
        mutable struct ::sline sl = {};
        igris::unbounded_array<char> _space = {};

    public:
        sline() = default;

        sline(size_t sz)
        {
            init(sz);
        }

        char *data()
        {
            return _space.data();
        }
        const char *data() const
        {
            return _space.data();
        }

        void init(size_t sz)
        {
            _space.resize(sz);
            ::sline_init(&sl, _space.data(), sz);
        }

        void newdata(const char *data, size_t sz)
        {
            ::sline_newdata(&sl, data, sz);
        }

        void newdata(char data)
        {
            ::sline_putchar(&sl, data);
        }

        const char *getline() const
        {
            return ::sline_getline(&sl);
        }

        size_t current_size() const
        {
            return sl.len;
        }

        size_t maximum_size() const
        {
            return ::sline_size(&sl);
        }

        size_t storage_size() const
        {
            return _space.size();
        }

        void reset()
        {
            ::sline_reset(&sl);
        }

        void clear()
        {
            for (auto &it : _space)
                it = 0;
        }

        void set_size_and_cursor(size_t sz, size_t cursor)
        {
            sl.len = sz;
            sl.cursor = cursor;
        }

        bool equal(const char *str) const
        {
            return ::sline_equal(&sl, str);
        }

        auto backspace(int i)
        {
            return ::sline_backspace(&sl, i);
        }

        auto right()
        {
            return ::sline_right(&sl);
        }

        auto left()
        {
            return ::sline_left(&sl);
        }

        auto del(int i)
        {
            return ::sline_delete(&sl, i);
        }

        auto rightsize()
        {
            return ::sline_rightsize(&sl);
        }

        auto rightpart()
        {
            return ::sline_rightpart(&sl);
        }

        bool in_rightpos()
        {
            return ::sline_in_rightpos(&sl);
        }
    };
}

#endif