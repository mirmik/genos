
#include <nos/log/target_logger.h>

void nos::log::target_logger::log(level lvl,
                                  const std::string_view &msgfmt,
                                  const visitable_arglist &arglist)
{
    if (has_one_who_need(lvl))
    {
        std::string msg = nos::format(msgfmt, arglist);
        for (nos::log::target &target : _targets)
            target.log(lvl, msg);
    }
}

bool nos::log::target_logger::has_one_who_need(nos::log::level lvl)
{
    return std::any_of(
        _targets.begin(), _targets.end(), [&](nos::log::target &target) {
            return target.minimal_level() <= lvl;
        });
}