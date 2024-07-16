#ifndef CROW_NOS_LOGGER_H
#define CROW_NOS_LOGGER_H

#include <crow/nodes/publisher_node.h>
#include <nos/log/logger.h>
#include <nos/timestamp.h>

namespace crow
{
    class publish_logger : public nos::log::logger
    {
    public:
        crow::publisher_node *pubnode;

    public:
        publish_logger() = default;
        publish_logger(const publish_logger &) = default;
        publish_logger(publish_logger &&) = default;

        publish_logger &operator=(const publish_logger &) = default;
        publish_logger &operator=(publish_logger &&) = default;

        publish_logger(const std::string &_name, crow::publisher_node *pubnode)
            : logger(_name), pubnode(pubnode)
        {
        }

        void init(const std::string &_name, crow::publisher_node *pubnode)
        {
            logger::set_name(_name);
            this->pubnode = pubnode;
        }

        void log(nos::log::level lvl,
                 const std::string_view &fmt,
                 const nos::visitable_arglist &arglist) override
        {
            auto str = nos::format(fmt, arglist) + "\r\n";
            pubnode->publish({str.data(), str.size()});
        }

        using nos::log::logger::log;
    };
} // namespace crow

#endif
