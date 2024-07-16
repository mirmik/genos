#include <nos/fprint.h>
#include <nos/log/logger.h>

std::string nos::log::logger::name() const
{
    return _name;
}

std::string nos::log::logger::pattern() const
{
    return _pattern;
}

nos::log::logger &nos::log::logger::set_pattern(const std::string &pattern)
{
    this->_pattern = pattern;
    return *this;
}

nos::log::logger &nos::log::logger::set_name(const std::string &name)
{
    this->_name = name;
    return *this;
}

void nos::log::logger::log(level lvl,
                           const std::string_view &msgfmt,
                           const visitable_arglist &arglist)
{
    if (lvl < minlevel())
        return;

    using namespace nos::argument_literal;
    auto msg = nos::format(msgfmt, arglist);
    nos::fprint(_pattern,
                "level"_a = nos::log::level_to_string(lvl),
                "logger"_a = _name,
                "msg"_a = msg);
}
