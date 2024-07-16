#ifndef NOS_IO_BUFFER_READER_H
#define NOS_IO_BUFFER_READER_H

#include <iostream>

namespace nos
{
    class buffer_reader : public nos::istream
    {
    private:
        char *buf;
        size_t len;

    public:
        buffer_reader(char *buf, size_t len) : buf(buf), len(len) {}

        nos::expected<size_t, nos::input_error> read(void *ptr,
                                                     size_t sz) override
        {
            if (len == 0)
                return nos::input_error::eof();

            int rd = len > sz ? sz : len;
            len -= rd;

            memcpy(ptr, buf, rd);
            buf += rd;

            return rd;
        }
    };
}

#endif
