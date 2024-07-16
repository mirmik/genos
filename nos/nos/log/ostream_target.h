#ifndef NOS_LOG_OSTREAM_TARGET_H
#define NOS_LOG_OSTREAM_TARGET_H

#include <nos/io/ostream.h>
#include <nos/log/ilogger.h>
#include <nos/log/target.h>

namespace nos
{
    namespace log
    {
        class ostream_target : public nos::log::target
        {
            nos::ostream *_stream = nullptr;
            std::string _pattern =
                std::string("[{level}] {msg}") + newline_string;

        public:
            ostream_target() : nos::log::target(nos::log::level::Trace){};

            ostream_target(nos::ostream &stream,
                           nos::log::level minlevel = nos::log::level::Trace)
                : nos::log::target(minlevel), _stream(&stream)
            {
            }
            ostream_target(const ostream_target &) = default;
            ostream_target &operator=(const ostream_target &) = default;

            ~ostream_target() = default;

            void set_ostream(nos::ostream &stream)
            {
                _stream = &stream;
            }

            void set_pattern(const std::string &pattern)
            {
                _pattern = pattern;
            }

            void log(level lvl, const std::string_view &msg) override;
        };
    }
}

#endif