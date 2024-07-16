#include <crow/gateway.h>
#include <crow/tower.h>

#include <igris/sync/syslock.h>

DLIST_HEAD(crow::gateway_list);

int crow::gateway::bind(int id)
{
    gateway *g;

    system_lock();
    g = get_gateway(id);
    if (g)
        return -1;

    this->id = id;
    dlist_add_tail(&this->lnk, &crow::gateway_list);
    system_unlock();

    return 0;
}

crow::gateway *crow::get_gateway(int no)
{
    crow::gateway * g;
    dlist_for_each_entry(g, &crow::gateway_list, lnk)
    {
        if (g->id == no)
            return g;
    }

    return nullptr;
}