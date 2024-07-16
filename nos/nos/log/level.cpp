#include <nos/log/level.h>

const char *nos::log::level_to_string(level lvl)
{
    switch (lvl)
    {
    case level::Trace:
        return "trace";
    case level::Debug:
        return "debug";
    case level::Info:
        return " info";
    case level::Warn:
        return " warn";
    case level::Error:
        return "error";
    case level::Fault:
        return "fault";
    }
    return nullptr;
}

nos::log::level nos::log::level_from_string(const nos::buffer &str)
{
    if (str == "fault")
        return level::Fault;
    else if (str == "error")
        return level::Error;
    else if (str == "warn")
        return level::Warn;
    else if (str == "info")
        return level::Info;
    else if (str == "debug")
        return level::Debug;
    else if (str == "trace")
        return level::Trace;

    return level::Trace;
}
