#ifndef NOS_LOGGER_H
#define NOS_LOGGER_H

#include <nos/fprint.h>
#include <nos/log/ilogger.h>
#include <nos/log/level.h>
#include <nos/print.h>
#include <string>
#include <string_view>

namespace nos
{
    namespace log
    {
        class logger : public ilogger
        {
        public:
            std::string _name = "undefined";
            std::string _pattern =
                std::string("[{level}] {msg}") + newline_string;

        public:
            logger() = default;
            logger(const logger &) = default;
            logger(logger &&) = default;
            logger &operator=(const logger &) = default;
            logger &operator=(logger &&) = default;
            logger(const std::string &_name) : _name(_name) {}

            std::string name() const;

            std::string pattern() const;

            logger &set_name(const std::string &_name);

            logger &set_pattern(const std::string &pattern);

            void log(level lvl,
                     const std::string_view &msgfmt,
                     const visitable_arglist &arglist) override;
            virtual ~logger() = default;

            using ilogger::log;

            logger &set_level(level lvl)
            {
                ilogger::set_level(lvl);
                return *this;
            }

            logger &set_pattern(const char *pattern)
            {
                _pattern = pattern;
                return *this;
            }
        };
    }
}

#endif
