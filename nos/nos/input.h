/** @file */

#ifndef NOS_INPUT_H
#define NOS_INPUT_H

#include <chrono>
#include <nos/buffer.h>
#include <nos/input/input_error.h>
#include <nos/util/error.h>
#include <nos/util/expected.h>
#include <stdlib.h>
#include <string>
#include <string_view>

namespace nos
{
    class istream;
    extern istream *current_istream;

    // main api
    nos::expected<std::string, nos::input_error> read_from(nos::istream &is,
                                                           size_t sz);

    nos::expected<std::string, nos::input_error> readline_from(
        nos::istream &is, size_t maxsz = 1024, bool include_newline = true);

    nos::expected<std::string, nos::input_error> readall_from(nos::istream &is);

    nos::expected<std::string, nos::input_error>
    read_until_from(nos::istream &is,
                    size_t maxsz,
                    const std::string_view &delimiters,
                    bool include_delimiter = true);

    // buffer api
    nos::expected<size_t, nos::input_error>
    read_paired_from(nos::istream &is,
                     char *buf,
                     size_t buflen,
                     char a,
                     char b,
                     bool ignore = true);

    nos::expected<size_t, nos::input_error>
    read_until_from(nos::istream &is,
                    nos::buffer buf,
                    const std::string_view &delimiters,
                    bool include_delimiter = true);

    // std stream api
    nos::expected<std::string, nos::input_error>
    readline(size_t maxsz = 1024, bool include_newline = true);

}

#include <nos/io/istream.h>

#endif
