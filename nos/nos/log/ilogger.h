#ifndef NOS_LOG_ILOGGER_H
#define NOS_LOG_ILOGGER_H

#include <nos/fprint.h>
#include <nos/log/ilogger.h>
#include <nos/log/level.h>
#include <string>
#include <string_view>

namespace nos
{
    namespace log
    {
        class ilogger
        {
            level _minlevel = level::Trace;

        public:
            ilogger() = default;
            virtual ~ilogger() = default;

            virtual void log(level lvl,
                             const std::string_view &msgfmt,
                             const visitable_arglist &arglist) = 0;

            level minlevel() const
            {
                return _minlevel;
            }

            void set_level(level lvl)
            {
                _minlevel = lvl;
            }

            template <typename... Args>
            inline void
            log(level lvl, const std::string_view &fmt, const Args &... args)
            {
                log(lvl,
                    fmt,
                    visitable_arglist(
                        {visitable_argument(args, nos::format_visitor())...}));
            }

            template <typename... Args> inline void trace(const Args &... args)
            {
                log(level::Trace, args...);
            }
            template <typename... Args> inline void debug(const Args &... args)
            {
                log(level::Debug, args...);
            }
            template <typename... Args> inline void info(const Args &... args)
            {
                log(level::Info, args...);
            }
            template <typename... Args> inline void warn(const Args &... args)
            {
                log(level::Warn, args...);
            }
            template <typename... Args> inline void error(const Args &... args)
            {
                log(level::Error, args...);
            }
            template <typename... Args> inline void fault(const Args &... args)
            {
                log(level::Fault, args...);
            }
        };

        void set_default_logger(ilogger *logger);
        ilogger *default_logger();

        template <typename... Args>
        inline void
        log(level lvl, const std::string_view &fmt, const Args &... args)
        {
            if (default_logger())
                default_logger()->log(lvl,
                                      fmt,
                                      visitable_arglist({visitable_argument(
                                          args, nos::format_visitor())...}));
        }

        template <typename... Args> inline void trace(const Args &... args)
        {
            log(level::Trace, args...);
        }
        template <typename... Args> inline void debug(const Args &... args)
        {
            log(level::Debug, args...);
        }
        template <typename... Args> inline void info(const Args &... args)
        {
            log(level::Info, args...);
        }
        template <typename... Args> inline void warn(const Args &... args)
        {
            log(level::Warn, args...);
        }
        template <typename... Args> inline void error(const Args &... args)
        {
            log(level::Error, args...);
        }
        template <typename... Args> inline void fault(const Args &... args)
        {
            log(level::Fault, args...);
        }

    }
}

#endif