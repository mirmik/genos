#include <nos/io/ostream.h>
#include <nos/print/stdtype.h>

nos::expected<size_t, nos::output_error> nos::ostream::fill(char c, size_t len)
{
    size_t _len = len;
    while (_len--)
    {
        this->putbyte(c);
    }

    return len;
}

nos::expected<size_t, nos::output_error>
nos::ostream::write_upper(const void *_ptr, size_t sz)
{
    char *ptr = (char *)_ptr;
    size_t _sz = sz;
    while (_sz--)
        putbyte(toupper(*ptr++));
    return sz;
}

nos::expected<size_t, nos::output_error>
nos::ostream::write_lower(const void *_ptr, size_t sz)
{
    char *ptr = (char *)_ptr;
    size_t _sz = sz;
    while (_sz--)
        putbyte(tolower(*ptr++));
    return sz;
}
