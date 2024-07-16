#ifndef NOS_IO_ARRAY_OSTREAM_H
#define NOS_IO_ARRAY_OSTREAM_H

#include <nos/binary/bytearray.h>
#include <nos/io/ostream.h>

namespace nos
{
    class bytearray_ostream : public nos::ostream
    {
        bytearray &array;

    public:
        bytearray_ostream(bytearray &arr) : array(arr) {}

        nos::expected<size_t, nos::output_error> write(const void *ptr,
                                                       size_t sz) override
        {
            array.append((uint8_t *)ptr, sz);
            return sz;
        }
    };
}

#endif