#ifndef NOS_TEST_WIDTH_STREAM_H
#define NOS_TEST_WIDTH_STREAM_H

#include <nos/io/ostream.h>

namespace nos
{
    class test_width_ostream : public nos::ostream
    {
    public:
        size_t result = 0;

    public:
        test_width_ostream() {}

        nos::expected<size_t, nos::output_error> write(const void *ptr,
                                                       size_t sz) override
        {
            (void)ptr;
            result += sz;
            return sz;
        }

        size_t room()
        {
            return 1024;
        }
    };
}

#endif