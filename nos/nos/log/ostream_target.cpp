#include <nos/log/ostream_target.h>

using namespace nos::argument_literal;

void nos::log::ostream_target::log(level lvl, const std::string_view &msg)
{
    nos::fprint_to(*_stream,
                   _pattern,
                   "level"_a = nos::log::level_to_string(lvl),
                   "msg"_a = msg);
}