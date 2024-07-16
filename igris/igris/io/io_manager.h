#ifndef IGRIS_IO_MANAGER_H
#define IGRIS_IO_MANAGER_H

namespace igris
{
    enum EventType
    {
        AVAILABLE = 0,
        ACCEPTED = 1,
    }

    class what_you_whish
    {
        int fd;
        EventType igris::delegate<void, void *> action;
    };

    class poll_manager
    {
        std::list<what_you_whish> events;

        void poll(double timeout) {}
    };
}

#endif