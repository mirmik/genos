#ifndef NOS_FPRINT_STDTYPE_H
#define NOS_FPRINT_STDTYPE_H

/**
    @file
*/

#include <nos/input/input_error.h>
#include <nos/util/buffer.h>
#include <nos/util/expected.h>
#include <string>

namespace nos
{
    class ostream;
}

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, const char *obj, const nos::buffer &opts);
nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, char *obj, const nos::buffer &opts);

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, bool obj, const nos::buffer &opts);

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, signed char obj, const nos::buffer &opts);
nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, signed short obj, const nos::buffer &opts);
nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, signed int obj, const nos::buffer &opts);
nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, signed long obj, const nos::buffer &opts);
nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, signed long long obj, const nos::buffer &opts);

nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, unsigned char obj, const nos::buffer &opts);
nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, unsigned short obj, const nos::buffer &opts);
nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, unsigned int obj, const nos::buffer &opts);
nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, unsigned long obj, const nos::buffer &opts);
nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, unsigned long long obj, const nos::buffer &opts);
nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, double obj, const nos::buffer &opts);
nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, float obj, const nos::buffer &opts);
nos::expected<size_t, nos::output_error>
nos_fprint(nos::ostream &os, const std::string &obj, const nos::buffer &opts);

#include <nos/io/ostream.h>

#endif
