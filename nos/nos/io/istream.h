#ifndef NOS_IO_ISTREAM_H
#define NOS_IO_ISTREAM_H

#include <chrono>
#include <nos/input/input_error.h>
#include <nos/util/expected.h>
#include <stdlib.h>
#include <string>

using namespace std::chrono_literals;

namespace nos
{
    class istream
    {
    public:
        virtual ~istream() = default;
        virtual nos::expected<size_t, nos::input_error> read(void *ptr,
                                                             size_t sz) = 0;

        nos::expected<size_t, nos::input_error> ignore()
        {
            char c;
            return read(&c, 1);
        }

        nos::expected<size_t, nos::input_error> ignore(size_t i)
        {
            size_t j = i;
            while (j--)
            {
                auto r = ignore();
                if (r.is_error())
                {
                    return r;
                }
            }
            return i;
        }
    };
}

#endif
