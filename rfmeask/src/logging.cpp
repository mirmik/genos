#include <comm/Notify.h>
#include <logging.h>
#include <nos/log/target_logger.h>

std::recursive_mutex preset_protector_mutex;

class MessageChannelTarget : public nos::log::target
{
    NotifyTheme *theme;

public:
    MessageChannelTarget(NotifyTheme *theme) :
        nos::log::target(nos::log::level::Trace), theme(theme)
    {
    }

    void log(nos::log::level lvl, const std::string_view &msg) override;
};

void MessageChannelTarget::log(nos::log::level lvl, const std::string_view &msg)
{
    if (lvl < minimal_level())
        return;

    using namespace nos::argument_literal;
    /*nos::fprint(_pattern,
                "level"_a = nos::log::level_to_string(lvl),
                "logger"_a = _name,
                "msg"_a = msg);*/

    theme->notify(msg);
}

std::unique_ptr<MessageChannelTarget> message_channel_target;
nos::log::ostream_target ostream_logger;
nos::log::target_logger system_logger;

void init_logging_system(std::string_view loglevel, bool force_debug)
{
    ostream_logger.set_ostream(*nos::default_ostream());

    message_channel_target =
        std::make_unique<MessageChannelTarget>(messageChannelNotify.get());

    // system_logger.add_target(*message_channel_target.get());
    system_logger.add_target(ostream_logger);

    system_logger.set_level(force_debug
                                ? nos::log::level::Trace
                                : nos::log::level_from_string(loglevel));

    nos::log::set_default_logger(&system_logger);
}