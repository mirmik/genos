#ifndef RALGO_INFO_H
#define RALGO_INFO_H

#include <nos/log/logger.h>

namespace ralgo
{
    extern nos::log::ilogger *logger;

    enum LogLevel
    {
        RALGO_DEBUG,
        RALGO_INFO,
        RALGO_WARN,
        RALGO_FAULT,
    };

    inline nos::log::level level_to_noslvl(LogLevel lvl)
    {
        switch (lvl)
        {
        // case RALGO_TRACE:
        //    return nos::log::level::Trace;
        case RALGO_DEBUG:
            return nos::log::level::Debug;
        case RALGO_INFO:
            return nos::log::level::Info;
        case RALGO_WARN:
            return nos::log::level::Warn;
        case RALGO_FAULT:
            return nos::log::level::Error;
        }
        return nos::log::level::Debug;
    }

    void log(LogLevel lvl, const char *, const char *, const char *);
    void debug(const char *, const char *, const char *);
    void info(const char *, const char *, const char *);
    void warn(const char *, const char *, const char *);
    void fault(const char *, const char *, const char *);

    void log(LogLevel lvl, const char *, const char *);
    void debug(const char *, const char *);
    void info(const char *, const char *);
    void warn(const char *, const char *);
    void fault(const char *, const char *);

    void log(LogLevel lvl, const char *);
    void debug(const char *);
    void info(const char *);
    void warn(const char *);
    void fault(const char *);

    template <class... Args>
    void logf(LogLevel lvl, const std::string_view &fmt, Args &&... args)
    {
        if (logger)
            logger->log(level_to_noslvl(lvl), fmt, std::forward<Args>(args)...);
    }

    template <class... Args>
    void debugf(const std::string_view &fmt, Args &&... args)
    {
        if (logger)
            logger->debug(fmt, std::forward<Args>(args)...);
    }

    template <class... Args>
    void infof(const std::string_view &fmt, Args &&... args)
    {
        if (logger)
            logger->info(fmt, std::forward<Args>(args)...);
    }

    template <class... Args>
    void warnf(const std::string_view &fmt, Args &&... args)
    {
        if (logger)
            logger->warn(fmt, std::forward<Args>(args)...);
    }

    template <class... Args>
    void faultf(const std::string_view &fmt, Args &&... args)
    {
        if (logger)
            logger->error(fmt, std::forward<Args>(args)...);
    }

    void set_logger(nos::log::ilogger *logger);
}

#endif
