#include <ralgo/log.h>
#include <string.h>
#include <string>
#include <unistd.h>

void ralgo::log(ralgo::LogLevel lvl,
                const char *a,
                const char *b,
                const char *c)
{
    if (ralgo::logger == nullptr)
    {
        return;
    }

    std::string str = "";
    str += std::string(a);
    if (b)
    {
        str += " ";
        str += std::string(b);
    }
    if (c)
    {
        str += " ";
        str += std::string(c);
    }

    switch (lvl)
    {
    case RALGO_DEBUG:
        ralgo::logger->debug(str);
        break;
    case RALGO_INFO:
        ralgo::logger->info(str);
        break;
    case RALGO_WARN:
        ralgo::logger->warn(str);
        break;
    case RALGO_FAULT:
        ralgo::logger->error(str);
        break;
    }
}
