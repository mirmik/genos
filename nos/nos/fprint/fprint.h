#ifndef NOS_FPRINT_FPRINT_H
#define NOS_FPRINT_FPRINT_H

/**
    @file
*/

#include <nos/fprint/stdtype.h>
#include <nos/fprint/visitor.h>
#include <nos/io/string_writer.h>
#include <nos/print/print.h>

namespace nos
{
    nos::expected<size_t, nos::output_error>
    fprint_impl(nos::ostream &out,
                const std::string_view &fmt,
                const visitable_arglist &args);

    template <typename... Args>
    nos::expected<size_t, nos::output_error> fprint_to(
        nos::ostream &out, const std::string_view &fmt, const Args &... args)
    {
        return *fprint_impl(out,
                            fmt,
                            visitable_arglist({visitable_argument(
                                args, nos::format_visitor())...}));
    }

    template <typename... Args>
    nos::expected<size_t, nos::output_error> fprint(const std::string_view &fmt,
                                                    const Args &... args)
    {
        return *nos::fprint_to(*current_ostream, fmt, args...);
    }

    template <typename... Args>
    nos::expected<size_t, nos::output_error> fprintln(const Args &... args)
    {
        size_t ret = 0;
        ret += *fprint_to(*current_ostream, args...);
        ret += *println_to(*current_ostream);
        return ret;
    }

    template <typename... Args>
    nos::expected<size_t, nos::output_error> fprintln_to(nos::ostream &out,
                                                         const Args &... args)
    {
        size_t ret = 0;
        ret += *fprint_to(out, args...);
        ret += *println_to(out);
        return ret;
    }

    nos::expected<size_t, nos::output_error> fprint(const char *arg);

    nos::expected<size_t, nos::output_error> fprintln(const char *arg);

    nos::expected<size_t, nos::output_error> fprint_to(nos::ostream &out,
                                                       const char *arg);

    nos::expected<size_t, nos::output_error> fprintln_to(nos::ostream &out,
                                                         const char *arg);

    inline std::string format(const std::string_view &fmt,
                              const visitable_arglist &args)
    {
        std::string ret;
        nos::string_writer writer(ret);
        nos::fprint_impl(writer, fmt, args);
        return ret;
    }

    template <typename... Args>
    std::string format(const std::string_view &fmt, const Args &... args)
    {
        std::string ret;
        nos::string_writer writer(ret);
        nos::fprint_to(writer, fmt, args...);
        return ret;
    }

    static inline int format_buffer(char *buf,
                                    const std::string_view &fmt,
                                    const visitable_arglist &args)
    {
        nos::cstring_writer writer(buf);
        int ret = *nos::fprint_impl(writer, fmt, args);
        writer.putbyte('\0');
        return ret;
    }

    template <typename... Args>
    int
    format_buffer(char *buf, const std::string_view &fmt, const Args &... args)
    {
        nos::cstring_writer writer(buf);
        int ret = *nos::fprint_to(writer, fmt, args...);
        writer.putbyte('\0');
        return ret;
    }
}

#endif
