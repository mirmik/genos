#include <igris/container/dlist.h>

void igris::dlist_node::unlink()
{
    if (is_linked())
    {
        next->prev = prev;
        prev->next = next;
        next = prev = this;
    }
}