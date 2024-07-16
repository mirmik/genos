#include <nos/input.h>
#include <nos/io/istream.h>
#include <nos/print.h>
#include <nos/util/error.h>
#include <nos/util/expected.h>
#include <stdexcept>

nos::expected<std::string, nos::input_error>
nos::readline_from(nos::istream &is, size_t maxsz, bool include_newline)
{
    char c;
    std::string str;

    while (1)
    {
        auto ret = is.read(&c, 1);

        if (ret.is_error())
            return ret.error();

        if (c == '\r')
        {
            if (include_newline)
                str += c;
            continue;
        }

        if (c == '\n')
        {
            if (include_newline)
                str += c;
            return str;
        }

        str.push_back(c);
        if (str.size() >= maxsz)
            return str;
    }
}

nos::expected<std::string, nos::input_error>
nos::read_until_from(nos::istream &is,
                     std::size_t max_size,
                     const std::string_view &delimiters,
                     bool include_delimiter)
{
    std::string ret;

    while (true)
    {
        char c;
        auto sts = is.read(&c, 1);

        if (sts.is_error())
        {
            return sts.error();
        }

        if (delimiters.find(c) != std::string::npos)
        {
            if (include_delimiter)
            {
                ret.push_back(c);
            }
            return ret;
        }

        ret.push_back(c);

        if (ret.size() >= max_size)
        {
            return ret;
        }
    }
}

nos::expected<size_t, nos::input_error>
nos::read_until_from(nos::istream &is,
                     nos::buffer buf,
                     const std::string_view &delimiters,
                     bool include_delimiter)
{
    char c;
    int count = 0;
    char *it = buf.data();
    char *eit = buf.data() + buf.size() - 1; // -1 for null terminator

    while (it != eit)
    {
        auto readed = is.read(&c, 1);

        if (readed.is_error())
        {
            return readed.error();
        }

        if (delimiters.find(c) != std::string::npos)
        {
            if (include_delimiter)
            {
                *it++ = c;
            }

            *it = '\0';
            return count;
        }

        *it++ = c;
        count++;
    }

    *it = 0;
    return count;
}

/// Считывает из потока ввода содержимое строки между открывающейся скобкой 'a'
/// и закрывающейся скобкой 'b' Параметр ignore позволяет отбростить данные
/// перед первой скобкой.
nos::expected<size_t, nos::input_error> nos::read_paired_from(
    nos::istream &is, char *buf, size_t buflen, char a, char b, bool ignore)
{
    char c;
    int count = 0;
    char *last = buf + buflen - 1;

    int paircount = 0;

    if (ignore)
        do
        {
            auto ret = is.read(&c, 1);
            if (ret.is_error())
                return ret.error();

            if (*ret <= 0)
                return ret;

        } while (c != a);
    else
    {
        auto ret = is.read(&c, 1);
        if (ret.is_error())
            return ret.error();

        if (c != a)
            return -1;
    }

    paircount = 1;
    *buf++ = c;
    count++;

    while (paircount != 0 && buf != last)
    {
        auto ret = is.read(&c, 1);
        if (ret.is_error())
            return ret.error();

        if (c == a)
        {
            ++paircount;
        }

        if (c == b)
        {
            if (--paircount == 0)
            {
                *buf++ = c;
                count++;
                *buf = 0;
                return count;
            }
        }

        *buf++ = c;
        count++;
    }

    *buf = 0;
    return count;
}

nos::expected<std::string, nos::input_error> nos::readall_from(nos::istream &is)
{
    std::string ret;
    char buf[1024];
    while (true)
    {
        auto ans = is.read(buf, sizeof(buf));
        if (ans.is_error())
        {
            return ret;
        }
        ret.append(buf, ans.value());
    }
    return ret;
}

nos::expected<std::string, nos::input_error> nos::readline(size_t maxsz,
                                                           bool include_newline)
{
    return nos::readline_from(*nos::current_istream, maxsz, include_newline);
}

nos::expected<std::string, nos::input_error> nos::read_from(nos::istream &is,
                                                            size_t size)
{
    std::string ret;
    ret.resize(size);
    auto result = is.read(&ret[0], size);
    if (!result.is_error())
    {
        ret.resize(*result);
        return ret;
    }
    else
    {
        return result.error();
    }
}
