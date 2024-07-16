#include <crow/gateway.h>
#include <crow/tower.h>

#include <igris/sync/syslock.h>

DLIST_HEAD(crow::gateway_list);
DLIST_HEAD(crow_gateway_list);

int crow_gateway_bind(struct crow_gateway *gate, int id)
{
    system_lock();
    if (crow_get_gateway(id))
        return -1;

    gate->id = id;
    dlist_move_tail(&gate->lnk, &crow_gateway_list);
    system_unlock();

    return 0;
}

struct crow_gateway *crow_get_gateway(int no)
{
    crow_gateway *g;
    dlist_for_each_entry(g, &crow_gateway_list, lnk)
    {
        if (g->id == no)
            return g;
    }

    return nullptr;
}

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
    crow::gateway *g;
    dlist_for_each_entry(g, &crow::gateway_list, lnk)
    {
        if (g->id == no)
            return g;
    }

    return nullptr;
}