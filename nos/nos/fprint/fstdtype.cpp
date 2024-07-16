#include <nos/fprint/spec.h>
#include <nos/fprint/stdtype.h>
#include <nos/input/input_error.h>
#include <nos/util/expected.h>
#include <nos/util/numconvert.h>

nos::expected<size_t, nos::output_error> nos_fprint(nos::ostream &os,
                                                    const char *text,
                                                    size_t size,
                                                    const nos::basic_spec &spec)
{
    int ret = 0;

    int pre_fill_len = 0;
    char post_fill_len = 0;

    int difflen = (int)spec.width - (int)size;

    if (difflen > 0)
    {
        switch (spec.align)
        {
        case nos::alignment::left:
            post_fill_len = difflen;
            break;

        case nos::alignment::right:
            pre_fill_len = difflen;
            break;

        case nos::alignment::center:
            pre_fill_len = difflen / 2;
            post_fill_len = difflen / 2;

            if (difflen % 2)
                pre_fill_len++;

            break;
        }
    }

    if (pre_fill_len)
    {
        ret += *os.fill(spec.fill, pre_fill_len);
    }

    if (spec.tcase == nos::text_case::upper)
    {
        ret += *os.write_upper(text, size);
    }
    else if (spec.tcase == nos::text_case::lower)
    {
        ret += *os.write_lower(text, size);
    }
    else
    {
        ret += *os.write(text, size);
    }

    if (post_fill_len)
    {
        ret += *os.fill(spec.fill, post_fill_len);
    }

    return ret;
}

nos::expected<size_t, nos::output_error> nos_fprint_integer_impl(
    nos::ostream &os, char *buf, size_t len, const nos::integer_spec &spec)
{
    return nos_fprint(os, buf, len, spec);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, const char *text, const nos::basic_spec &spec)
{
    return nos_fprint(os, text, strlen(text), spec);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, const char *obj, const nos::buffer &opts)
{
    nos::text_spec spec(opts);
    return nos_fprint(os, obj, strlen(obj), spec);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, const std::string &obj, const nos::buffer &opts)
{
    nos::text_spec spec(opts);
    return nos_fprint(os, obj.data(), obj.size(), spec);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, char *obj, const nos::buffer &opts)
{
    nos::text_spec spec(opts);
    return nos_fprint(os, obj, strlen(obj), spec);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, bool obj, const nos::buffer &opts)
{
    return nos_fprint(os, obj ? "true" : "false", opts);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, signed char obj, const nos::buffer &opts)
{
    return nos_fprint(os, (signed int)obj, opts);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, signed short obj, const nos::buffer &opts)
{
    return nos_fprint(os, (signed int)obj, opts);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, signed int obj, const nos::buffer &opts)
{
    return nos_fprint(os, (signed long long)obj, opts);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, signed long obj, const nos::buffer &opts)
{
    return nos_fprint(os, (signed long long)obj, opts);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, signed long long obj, const nos::buffer &opts)
{
    nos::integer_spec spec(opts);
    char buf[64 + 1];
    int base = 10;
    if (spec.hexmode)
        base = 16;
    char *endptr = __nos_itoa(obj, buf, base);
    std::ptrdiff_t len = endptr - buf;
    return nos_fprint_integer_impl(os, buf, len, spec);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, unsigned char obj, const nos::buffer &opts)
{
    return nos_fprint(os, (unsigned long long)obj, opts);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, unsigned short obj, const nos::buffer &opts)
{
    return nos_fprint(os, (unsigned long long)obj, opts);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, unsigned int obj, const nos::buffer &opts)
{
    return nos_fprint(os, (unsigned long long)obj, opts);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, unsigned long int obj, const nos::buffer &opts)
{
    return nos_fprint(os, (unsigned long long)obj, opts);
}

nos::expected<size_t, nos::output_error> nos_fprint(nos::ostream &os,
                                                    unsigned long long int obj,
                                                    const nos::buffer &opts)
{
    nos::integer_spec spec(opts);
    char buf[64 + 1];
    int base = 10;
    if (spec.hexmode)
        base = 16;
    char *endptr = __nos_utoa(obj, buf, base);
    int len = (int)(endptr - buf);
    return nos_fprint_integer_impl(os, buf, len, spec);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, double obj, const nos::buffer &opts)
{
    std::ptrdiff_t len;
    nos::float_spec spec(opts);
    char buf[64 + 1];
    int after_dot = 6;

    memset(buf, 0, sizeof(buf));
    if (spec.after_dot != -1)
        after_dot = spec.after_dot;

    len = __nos_dtoa(obj, buf, after_dot) - buf;
    return nos_fprint(os, buf, (int)len, spec);
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, float obj, const nos::buffer &opts)
{
    return nos_fprint(os, static_cast<double>(obj), opts);
}