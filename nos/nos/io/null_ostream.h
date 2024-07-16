#ifndef NOS_IO_NULL_OSTREAM_H
#define NOS_IO_NULL_OSTREAM_H

/**
    @file
*/

#include <nos/io/ostream.h>

namespace nos
{
    class null_ostream : public nos::ostream
    {
    public:
        nos::expected<size_t, nos::output_error> write(const void *ptr,
                                                       size_t sz) override
        {
            return sz;
        }
    };
}

#endif