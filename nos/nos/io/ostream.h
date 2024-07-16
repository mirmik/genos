#ifndef NOS_IO_OSTREAM_H
#define NOS_IO_OSTREAM_H

#include <ctype.h>
#include <nos/input/input_error.h>
#include <nos/util/expected.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline char __nos_half2hex(uint8_t n)
{
    return (char)(n < 10 ? '0' + n : 'A' - 10 + n);
}

namespace nos
{
    class ostream
    {
    public:
        virtual nos::expected<size_t, nos::output_error> write(const void *ptr,
                                                               size_t sz) = 0;
        nos::expected<size_t, nos::output_error> write_upper(const void *ptr,
                                                             size_t sz);
        nos::expected<size_t, nos::output_error> write_lower(const void *ptr,
                                                             size_t sz);

        nos::expected<size_t, nos::output_error> put(uint8_t byte)
        {
            return write(&byte, 1);
        }

        nos::expected<size_t, nos::output_error> putbyte(char c)
        {
            return write(&c, 1);
        }

        nos::expected<size_t, nos::output_error> fill(char c, size_t len);

        virtual int flush()
        {
            return 0;
        }

        virtual ~ostream() = default;
    };
}

#endif
