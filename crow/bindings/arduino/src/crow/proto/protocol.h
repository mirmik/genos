#ifndef CROW_PROTOCOL_H
#define CROW_PROTOCOL_H

#include <crow/packet.h>
#include <crow/tower.h>
#include <igris/datastruct/dlist.h>

namespace crow
{
    class protocol
    {
      public:
        struct dlist_head lnk = DLIST_HEAD_INIT(lnk);
        uint8_t id;

        virtual void incoming(crow_packet *pack) = 0;
        virtual void undelivered(crow_packet *pack) { crow::release(pack); }
        virtual void onestep() {}

        protocol(int id) : id(id) { dlist_add_tail(&lnk, &crow::protocols); }
    };
} // namespace crow

#endif
