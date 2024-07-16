#ifndef NOS_LOG_TARGET_H
#define NOS_LOG_TARGET_H

#include <nos/log/ilogger.h>

namespace nos
{
    namespace log
    {
        class target
        {
            nos::log::level _minlevel = nos::log::level::Trace;

        public:
            target(nos::log::level minlevel)
            {
                _minlevel = minlevel;
            }

            virtual void log(level lvl, const std::string_view &msg) = 0;
            virtual ~target() = default;

            nos::log::level minimal_level() const
            {
                return _minlevel;
            }
        };
    }
}

#endif