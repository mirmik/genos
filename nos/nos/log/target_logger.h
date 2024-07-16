#ifndef NOS_TARGET_LOGGER_H
#define NOS_TARGET_LOGGER_H

#include <nos/log/ilogger.h>
#include <nos/log/level.h>
#include <nos/log/logger.h>
#include <nos/log/target.h>

namespace nos
{
    namespace log
    {
        class target_logger : public nos::log::ilogger
        {
            std::vector<std::reference_wrapper<nos::log::target>> _targets = {};

        public:
            target_logger() = default;
            target_logger(const target_logger &) = default;
            target_logger(target_logger &&) = default;

            target_logger &operator=(const target_logger &) = default;
            target_logger &operator=(target_logger &&) = default;

            target_logger(const std::initializer_list<
                          std::reference_wrapper<nos::log::target>> &targets)
                : _targets(targets)
            {
            }

            void log(level lvl,
                     const std::string_view &msgfmt,
                     const visitable_arglist &arglist) override;
            virtual ~target_logger() = default;

            void add_target(nos::log::target &target)
            {
                _targets.push_back(target);
            }
            void remove_target(nos::log::target &target)
            {
                _targets.erase(std::remove_if(_targets.begin(),
                                              _targets.end(),
                                              [&target](auto &t) {
                                                  return &t.get() == &target;
                                              }),
                               _targets.end());
            }

            bool has_one_who_need(level lvl);

            using ilogger::log;
        };
    }
}

#endif