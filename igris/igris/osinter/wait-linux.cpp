#include <igris/osinter/wait.h>
#include <igris/sync/syslock.h>
#include <igris/syncxx/event.h>
#include <igris/util/macro.h>

struct linux_waiter
{
    waiter w = {};
    igris::event event = {};
};

void __unwait_handler(void *arg)
{
    linux_waiter *waiter = (linux_waiter *)arg;
    waiter->event.signal();
}

int wait_current_schedee(igris::dlist_base *head, int priority, void **future)
{
    linux_waiter waiter;
    waiter.w.func = __unwait_handler;
    waiter.w.obj = reinterpret_cast<uintptr_t>(&waiter);

    system_lock();

    if (priority)
        head->move_front(waiter.w.lnk);
    else
        head->move_back(waiter.w.lnk);
    system_unlock();

    // auto save = system_lock_save();
    waiter.event.wait();
    // system_lock_restore(save);

    *future = (void *)waiter.w.future;
    return 0;
}

int unwait_schedee_waiter(waiter *w)
{
    linux_waiter *waiter = mcast_out(w, struct linux_waiter, w);
    waiter->event.signal();

    return 0;
}
