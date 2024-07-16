/**
    @file
*/

#ifndef igris_READLINE_H
#define igris_READLINE_H

#include <igris/event/multiple_delegate.h>
#include <stdlib.h>
#include <string_view>
class simple_readline
{
    char *m_data = nullptr;
    char *m_data_end = nullptr;
    char *m_cursor = nullptr;
    char lastsymb = 0;

    igris::delegate<void, const char *, size_t> m_line_handler = {};
    igris::delegate<int, simple_readline &> m_full_handler = {};

public:
    void reset()
    {
        m_cursor = m_data;
        lastsymb = 0;
    }

    bool is_full() { return m_cursor == m_data_end; }

public:
    void set_line_handler(igris::delegate<void, const char *, size_t> dlg)
    {
        m_line_handler = dlg;
    }

    void set_full_handler(igris::delegate<int, simple_readline &> dlg)
    {
        m_full_handler = dlg;
    }

    simple_readline() = default;
    simple_readline(std::string_view buf)
        : m_data((char *)buf.data()),
          m_data_end((char *)buf.data() + buf.size() - 1),
          m_cursor((char *)buf.data())
    {
    }
    simple_readline(const simple_readline&) = delete;
    simple_readline& operator=(const simple_readline&) = delete;

    int write(const char *str, size_t sz)
    {
        char *readptr = (char *)str;
        while (sz--)
        {
            while (is_full())
            {
                int ret = m_full_handler(*this);
                if (ret == -1)
                    return -1;
            }

            char c = *readptr++;

            if (c == '\n')
            {
                *m_cursor = 0;
                if (lastsymb == '\r')
                    *(m_cursor - 1) = 0;
                m_line_handler(m_data, m_cursor - m_data);
                reset();
                continue;
            }

            *m_cursor++ = c;
            lastsymb = c;
        }
        return 0;
    }

    size_t size() const { return m_cursor - m_data; }

    const char *data() const { return m_data; }

    char *line()
    {
        *m_cursor = 0;
        return m_data;
    }

    std::string_view to_buffer() { return std::string_view(m_data, size()); }
};

#endif
