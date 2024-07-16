#ifndef MESSAGE_BUS_H
#define MESSAGE_BUS_H

#include <comm/Notify.h>

extern std::unique_ptr<NotifyTheme> messageBusNotify;

template <class... Args>
void message_bus_notify(const char *fmt, Args &&... args)
{
    std::string msg = nos::format(fmt, std::forward<Args>(args)...);
    messageBusNotify->notify(msg.c_str());
}

template <class... Args>
void message_bus_notify(const std::string &fmt, Args &&... args)
{
    std::string msg = nos::format(fmt, std::forward<Args>(args)...);
    messageBusNotify->notify(msg.c_str());
}

#endif