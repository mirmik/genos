/**
    @file
*/

#ifndef igris_2_READLINE_H
#define igris_2_READLINE_H

#include <algorithm>
#include <stdlib.h>
//#include <utilxx/buffer.h>

namespace igris
{

    template <size_t N> class ReadLine
    {
        char line[N];
        size_t cursor;

    public:
        void init() { cursor = 0; };

        bool is_full() { return cursor == N; }

        int putc(unsigned char c)
        {
            if (is_full())
                return 0;
            line[cursor++] = c;
            return 1;
        }

        size_t size() const { return cursor; }

        size_t copy_to(char *str, size_t bufsz)
        {
            auto len = std::min<size_t>(bufsz, cursor);
            memcpy(str, line, len);
            return len;
        }

        const char *data() const { return line; };

        const char *c_str()
        {
            line[cursor] = 0;
            return line;
        };

        /*	std::string_view to_buffer() {
                return std::string_view(line, cursor);
            }*/

        ReadLine() : cursor(0){};
    };

}

#endif
