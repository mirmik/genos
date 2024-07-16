#ifndef CIRCULAR_RECTANGLE_STREAM_H
#define CIRCULAR_RECTANGLE_STREAM_H

#include <nos/io/ostream.h>
#include <nos/util/buffer.h>

namespace nos
{
    class circular_textframe : public nos::ostream
    {
        int cols = 0;
        int rows = 0;

        int cursor_row = 0;
        int cursor_col = 0;

        char *_data = nullptr;

    public:
        circular_textframe(int cols, int rows, char *buffer)
            : cols(cols), rows(rows), _data(buffer)
        {
            memset(_data, ' ', cols * rows);
        }

        circular_textframe(const circular_textframe &) = delete;
        circular_textframe &operator=(const circular_textframe &) = delete;

        void fixup()
        {
            while (cursor_col >= cols)
            {
                cursor_col -= cols;
                ++cursor_row;
            }
            while (cursor_row >= rows)
            {
                cursor_row -= rows;
            }
        }

        void clean_current_row()
        {
            for (char &c : get_string(0))
                c = ' ';
        }

        int putbyte(char c)
        {
            if (c == '\n')
            {
                cursor_col = 0;
                cursor_row++;
                fixup();
                clean_current_row();
                return 1;
            }

            int oldrow = cursor_row;
            _data[cursor_row * cols + cursor_col] = c;
            cursor_col++;
            fixup();

            if (cursor_row != oldrow)
                clean_current_row();

            return 1;
        }

        nos::expected<size_t, nos::output_error> write(const void *data,
                                                       size_t size) override
        {
            int ret = 0;
            const char *it = (const char *)data;
            const char *eit = (const char *)data + size;
            for (; it != eit; ++it)
            {
                ret += putbyte(*it);
            }
            return ret;
        }

        nos::buffer get_string(int i)
        {
            int rowidx = cursor_row - i;
            while (rowidx < 0)
                rowidx += rows;

            return nos::buffer(_data + rowidx * cols, cols);
        }

        char operator()(int col, int row)
        {
            return get_string(row)[col];
        }
    };
}

#endif