#include <igris/dprint.h>
#include <ralgo/log.h>

nos::log::ilogger *ralgo::logger;

void ralgo::log(ralgo::LogLevel lvl, const char *str, const char *a)
{
    ralgo::log(lvl, str, a, nullptr);
}

void ralgo::log(ralgo::LogLevel lvl, const char *str)
{
    ralgo::log(lvl, str, nullptr, nullptr);
}

void ralgo::debug(const char *str, const char *a, const char *b)
{
    ralgo::log(RALGO_DEBUG, str, a, b);
}

void ralgo::info(const char *str, const char *a, const char *b)
{
    ralgo::log(RALGO_INFO, str, a, b);
}

void ralgo::warn(const char *str, const char *a, const char *b)
{
    ralgo::log(RALGO_WARN, str, a, b);
}

void ralgo::fault(const char *str, const char *a, const char *b)
{
    ralgo::log(RALGO_FAULT, str, a, b);
}

void ralgo::debug(const char *str, const char *a)
{
    ralgo::log(RALGO_DEBUG, str, a);
}

void ralgo::info(const char *str, const char *a)
{
    ralgo::log(RALGO_INFO, str, a);
}

void ralgo::warn(const char *str, const char *a)
{
    ralgo::log(RALGO_WARN, str, a);
}

void ralgo::fault(const char *str, const char *a)
{
    ralgo::log(RALGO_FAULT, str, a);
}

void ralgo::debug(const char *str)
{
    ralgo::log(RALGO_DEBUG, str);
}

void ralgo::info(const char *str)
{
    ralgo::log(RALGO_INFO, str);
}

void ralgo::warn(const char *str)
{
    ralgo::log(RALGO_WARN, str);
}

void ralgo::fault(const char *str)
{
    ralgo::log(RALGO_FAULT, str);
}

void ralgo::set_logger(nos::log::ilogger *logger)
{
    ralgo::logger = logger;
}