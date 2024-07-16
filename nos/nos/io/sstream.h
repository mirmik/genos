#ifndef NOS_STRING_STREAM_H
#define NOS_STRING_STREAM_H

#include <nos/io/istream.h>
#include <nos/io/ostream.h>

namespace nos
{
    class stringstream : public nos::ostream, public nos::istream
    {
    private:
        std::string _str = {};
        size_t read_marker = 0;

    public:
        stringstream(const std::string &str) : _str(str) {}
        stringstream() {}

        nos::expected<size_t, nos::output_error> write(const void *ptr,
                                                       size_t sz) override
        {
            _str.append((char *)ptr, sz);
            return sz;
        }

        size_t room()
        {
            return _str.size() - read_marker;
        }

        nos::expected<size_t, nos::input_error> read(void *ptr,
                                                     size_t sz) override
        {
            size_t readed = room() > sz ? sz : room();
            if (readed <= 0)
                return nos::input_error::eof();
            memcpy(ptr, _str.data() + read_marker, readed);
            read_marker += readed;
            return readed;
        }

        std::string &str()
        {
            return _str;
        }
    };
}

#endif