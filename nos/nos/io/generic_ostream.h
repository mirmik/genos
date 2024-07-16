#ifndef NOS_IO_GENERIC_OSTREAM_H
#define NOS_IO_GENERIC_OSTREAM_H

#include <nos/io/ostream.h>

namespace nos
{
    class generic_ostream : public ostream
    {
    private:
        int (*_write)(const void *ptr, size_t sz);

    public:
        generic_ostream(int (*foo)(const void *ptr, size_t sz)) : _write(foo) {}

        nos::expected<size_t, nos::output_error> write(const void *ptr,
                                                       size_t sz)
        {
            return _write(ptr, sz);
        }
    };
}

#endif