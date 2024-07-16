#ifndef CROW_SOCKET_H
#define CROW_SOCKET_H

#include <crow/proto/node.h>

namespace crow
{
    class socket_base : public node
    {
    public:
        dlist_head q = DLIST_HEAD_INIT(q);

        void incoming_packet(crow_packet *pack)
        {
            dlist_add(&pack->ulnk, &q);
        }
    };

    class socket : public socket_base
    {
    public:
        socket(int nodeno)
        {
            this->bind(nodeno);
        }

        void undelivered_packet(crow_packet *pack)
        {
            crow::release(pack);
        }

        crow::packet_ptr send(int rid,
                              nos::buffer addr,
                              const char *data,
                              size_t len,
                              uint8_t qos,
                              uint16_t ackquant)
        {
            return node_send(id, rid, addr, nos::buffer(data, len), qos,
                             ackquant);
        }

        crow_packet *recv()
        {
            while (!dlist_empty(&q))
            {
                waitevent();
                dprln("unwait recv");
            }

            auto it = dlist_first_entry(&q, crow_packet, ulnk);
            dlist_del(&it->ulnk);
            return it;
        }
    };

    /*class socket_acceptor_basic : public socket_base
    {
        dlist_head q = DLIST_INIT(q);

    public:
        socket() {}

        virtual socket* socket_create() = 0;

        void incoming_packet(crow_packet *pack)
        {
            dlist_add(&pack->ulnk, &q);
        }
    };

    class socket_acceptor : public socket_acceptor_basic
    {
    public:
        virtual socket* socket_create(crow_packet * request)
        {
            auto addr = request.addr();
            socket* ret = new socket();
            ret->raddr_ptr = new uint8_t[addr.size()];
            memcpy(ret->raddr_ptr, addr.data(), addr.size());
            return ret;
        }
    };*/
} // namespace crow

#endif
