#include <cctype>
#include <nos/fprint/visitor.h>
#include <nos/io/ostream.h>
#include <nos/print.h>
#include <nos/util/arglist.h>

namespace nos
{
    nos::expected<size_t, nos::output_error> fprint(const char *arg)
    {
        return print(arg);
    }

    nos::expected<size_t, nos::output_error> fprintln(const char *arg)
    {
        return println(arg);
    }

    nos::expected<size_t, nos::output_error> fprint_to(nos::ostream &out,
                                                       const char *arg)
    {
        return print_to(out, arg);
    }

    nos::expected<size_t, nos::output_error> fprintln_to(nos::ostream &out,
                                                         const char *arg)
    {
        return println_to(out, arg);
    }

    static size_t fprint_format_argument(nos::ostream &out,
                                         const char *&fmt,
                                         const nos::visitable_arglist &list,
                                         uint8_t &argnum)
    {
        int ret;
        char *pend;
        assert(*fmt == '{');

        fmt++;

        // Interpret double '{' symbol as '{'.
        if (*fmt == '{')
        {
            out.putbyte('{');
            fmt++;
            return 1;
        }

        const visitable_argument *varg = nullptr;

        if (isalpha(*fmt) || *fmt == '_')
        {
            const char *count_ptr = fmt;
            int len = 0;

            while (isalpha(*count_ptr) || isdigit(*count_ptr) || *count_ptr == '_')
            {
                len++;
                count_ptr++;
            }

            varg = &list[nos::buffer(fmt, len)];
        }
        else if (isdigit(*fmt))
        {
            // varg = &list[atou32(fmt, 10, &pend)];
            varg = &list[strtoul(fmt, &pend, 10)];
        }
        else
        {
            varg = &list[argnum];
        }

        while (*fmt != '}' && *fmt != ':' && *fmt != 0)
            fmt++;

        switch (*fmt)
        {
        case '}':
            ret = *nos::format_visitor::visit(*varg, out, nos::buffer{});
            break;

        case ':':
            ++fmt;
            ret = *nos::format_visitor::visit(
                *varg, out, nos::buffer{fmt, (size_t)(strchr(fmt, '}') - fmt)});
            break;

        case 0:
            return -1;

        default:
            return -1;
        }

        while (*fmt != '}' && *fmt != 0)
            fmt++;

        fmt++;
        argnum++;
        return ret;
    }

    nos::expected<size_t, nos::output_error>
    fprint_impl(nos::ostream &out,
                const std::string_view &fmt,
                const visitable_arglist &args)
    {
        uint8_t argnum = 0;
        const char *fmtptr = fmt.data();
        const char *fmtptr_start = fmt.data();
        int64_t fmtsize = (int64_t)fmt.size();
        int ret = 0;

        while (*fmtptr != 0 && (fmtptr - fmtptr_start < fmtsize))
        {
            if (*fmtptr == '{')
            {
                ret += fprint_format_argument(out, fmtptr, args, argnum);
            }
            else
            {
                auto strttxt = fmtptr;

                while (*fmtptr != 0 && *fmtptr != '{' &&
                       (fmtptr - fmtptr_start < fmtsize))
                    fmtptr++;

                ret += *out.write(strttxt, fmtptr - strttxt);
            }
        }

        return ret;
    }
}
