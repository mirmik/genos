#ifndef NOS_LOG_LEVEL_H
#define NOS_LOG_LEVEL_H

#include <nos/util/buffer.h>

namespace nos
{
    namespace log
    {
        enum class level : uint8_t
        {
            Trace = 0,
            Debug = 1,
            Info = 2,
            Warn = 3,
            Error = 4,
            Fault = 5,
        };

        const char *level_to_string(level lvl);
        level level_from_string(const nos::buffer &str);
    }
}

#endif
