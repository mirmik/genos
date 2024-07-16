#include <crow/proto/node.h>
#include <igris/osinter/wait.h>

int crow::node::waitevent()
{
    intptr_t ret;
    wait_current_schedee(&waitlnk, 0, (void **)&ret);
    return ret;
}

void crow::node::notify_one(intptr_t future)
{
    unwait_one(&waitlnk, (intptr_t)future);
}

void crow::node::notify_all(intptr_t future)
{
    unwait_all(&waitlnk, (intptr_t)future);
}
