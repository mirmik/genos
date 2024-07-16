#include <nos/io/ostream.h>
#include <nos/print/print.h>
#include <nos/print/stdtype.h>
#include <string>
#include <string_view>

const char *nos::newline_string = "\r\n";

void nos::set_default_ostream(nos::ostream *out)
{
    nos::current_ostream = out;
}

nos::ostream *nos::default_ostream()
{
    return nos::current_ostream;
}

nos::expected<size_t, nos::output_error>
nos::write_to(nos::ostream &out, const void *buf, size_t sz)
{
    return out.write(buf, sz);
}

nos::expected<size_t, nos::output_error> nos::putbyte_to(nos::ostream &out,
                                                         char c)
{
    return out.write(&c, 1);
}

nos::expected<size_t, nos::output_error>
nos::writeln_to(nos::ostream &out, const void *buf, size_t sz)
{
    auto ret1 = out.write(buf, sz);
    if (ret1.is_error())
        return ret1.error();
    auto ret2 = nos::println_to(out);
    if (ret2.is_error())
        return ret2.error();
    return *ret1 + *ret2;
}

nos::expected<size_t, nos::output_error> nos::println_to(nos::ostream &o)
{
    return o.write(nos::newline_string, 2);
}

nos::expected<size_t, nos::output_error> nos::println()
{
    return nos::println_to(*nos::current_ostream);
}

void nos::flush()
{
    nos::current_ostream->flush();
}

void flush_to(nos::ostream &out)
{
    out.flush();
}

nos::expected<size_t, nos::output_error> nos::print_dump_to(
    nos::ostream &out, const void *mem, size_t len, unsigned int columns)
{
    size_t ret = 0;
    unsigned int i, j;

    if (len == 0)
    {
        ret += *out.write("0x", 2);
        ret += *nos::printptr_to(out, (void *)((char *)mem));
        ret += *out.putbyte(':');
        ret += *nos::println_to(out);
        return ret;
    }

    for (i = 0; i < len + ((len % columns) ? (columns - len % columns) : 0);
         i++)
    {
        // print offset
        if (i % columns == 0)
        {
            ret += *out.write("0x", 2);
            ret += *nos::printptr_to(out, (void *)((char *)mem + i));
            ret += *out.putbyte(':');
        }

        // print hex data
        if (i < len)
        {
            ret += *nos::printhex_to(out, ((char *)mem)[i]);
            ret += *out.putbyte(' ');
        }
        else
        {
            // end of block, just aligning for ASCII dump
            ret += *out.write("   ", 3);
        }

        // print ASCII dump
        if (i % columns == (columns - 1))
        {
            for (j = i - (columns - 1); j <= i; j++)
            {
                if (j >= len)
                {
                    // end of block, not really printing
                    ret += *out.putbyte(' ');
                }
                else if (isprint(((char *)mem)[j]))
                {
                    // printable char
                    ret += *out.putbyte((char)0xFF & ((char *)mem)[j]);
                }
                else
                {
                    // other char
                    ret += *out.putbyte('.');
                }
            }

            ret += *nos::println();
        }
    }

    return ret;
}

nos::expected<size_t, nos::output_error>
nos::print_dump(const void *ptr, size_t sz, unsigned int columns)
{
    return nos::print_dump_to(*nos::current_ostream, ptr, sz, columns);
}

nos::expected<size_t, nos::output_error>
nos::print_dump(const std::string_view &buf, unsigned int columns)
{
    return nos::print_dump_to(
        *nos::current_ostream, buf.data(), buf.size(), columns);
}

nos::expected<size_t, nos::output_error> nos::write(const void *buf, size_t sz)
{
    return nos::write_to(*nos::current_ostream, buf, sz);
}

nos::expected<size_t, nos::output_error> nos::putbyte(char c)
{
    return nos::putbyte_to(*nos::current_ostream, c);
}

nos::expected<size_t, nos::output_error> nos::writeln(const void *buf,
                                                      size_t sz)
{
    return nos::writeln_to(*nos::current_ostream, buf, sz);
}

nos::expected<size_t, nos::output_error>
nos::fill_to(nos::ostream &out, char c, size_t sz)
{
    size_t ret = 0;
    for (size_t i = 0; i < sz; i++)
    {
        auto r = out.putbyte(c);
        if (r.is_error())
            return r;
        ret += *r;
    }
    return ret;
}

nos::expected<size_t, nos::output_error>
nos::fill_to(nos::ostream &out, std::string_view &c, size_t sz)
{
    size_t ret = 0;
    for (size_t i = 0; i < sz; i++)
    {
        auto r = nos::print_to(out, c);
        if (r.is_error())
            return r;
        ret += *r;
    }
    return ret;
}

nos::expected<size_t, nos::output_error> nos::fill(char c, size_t sz)
{
    return nos::fill_to(*nos::current_ostream, c, sz);
}

nos::expected<size_t, nos::output_error> nos::fill(std::string_view &c,
                                                   size_t sz)
{
    return nos::fill_to(*nos::current_ostream, c, sz);
}

nos::expected<size_t, nos::output_error> nos::printhex_to(nos::ostream &out,
                                                          char c)
{
    char buf[2];
    buf[0] = __nos_half2hex((uint8_t)((c & 0xF0) >> 4));
    buf[1] = __nos_half2hex((uint8_t)(c & 0x0F));
    return out.write(buf, 2);
}

nos::expected<size_t, nos::output_error>
nos::printhex_to(nos::ostream &out, const void *ptr, size_t sz)
{
    size_t ret = 0;
    char *_ptr = (char *)ptr;
    for (int i = (int)sz - 1; i >= 0; i--)
    {
        auto ret1 = nos::printhex_to(out, _ptr[i]);
        if (ret1.is_error())
            return ret1.error();
        ret += ret1.value();
    }
    return ret;
}

nos::expected<size_t, nos::output_error> nos::printbin_to(nos::ostream &out,
                                                          char c)
{
    size_t ret = 0;
    for (int j = 7; j >= 0; --j)
    {
        ret += *out.putbyte((c & (1 << j)) ? '1' : '0');
    }
    return ret;
}

nos::expected<size_t, nos::output_error>
nos::printbin_to(nos::ostream &out, const void *ptr, size_t sz)
{
    size_t ret = 0;
    char *_ptr = (char *)ptr;
    for (int i = (int)sz - 1; i >= 0; --i)
    {
        auto ret1 = printbin_to(out, _ptr[i]);
        if (ret1.is_error())
            return ret1.error();
        ret += ret1.value();
    }
    return ret;
}

nos::expected<size_t, nos::output_error>
nos::printhex_to(nos::ostream &out, void *ptr, size_t sz)
{
    return nos::printhex_to(out, (const void *)ptr, sz);
}

nos::expected<size_t, nos::output_error>
nos::printbin_to(nos::ostream &out, void *ptr, size_t size)
{
    return nos::printbin_to(out, (const void *)ptr, size);
}
