#ifndef CROW_PROTO_RETRANSLER_H
#define CROW_PROTO_RETRANSLER_H

#include <crow/proto/node.h>
#include <crow/types.h>
#include <igris/event/delegate.h>

namespace crow
{
    class service_retransler : public crow::node
    {
        crow::hostaddr_view host_a;
        crow::hostaddr_view host_b;

        uint16_t aid;
        uint16_t bid;

        igris::delegate<void, service_retransler *, crow_packet *>
            error_handler;

      protected:
        void init(uint16_t aid, crow::hostaddr_view host_a, uint16_t bid,
                  crow::hostaddr_view host_b)
        {
            this->aid = aid;
            this->bid = bid;
            this->host_a = host_a;
            this->host_b = host_b;

            bind();
        }

        void opposite_address(crow_packet *pack, nid_t &rid,
                              crow::hostaddr_view &host)
        {
            auto sh = crow::node::subheader(pack);

            if (aid == sh->sid && host_a == pack->addr())
            {
                host = host_b;
                rid = bid;
            }

            if (bid == sh->sid && host_b == pack->addr())
            {
                host = host_a;
                rid = aid;
            }
        }

        void incoming_packet(crow_packet *pack)
        {
            nid_t rid;
            crow::hostaddr_view host;

            opposite_address(pack, rid, host);
            node::send(rid, host, crow::node_data(pack), pack->header.qos,
                       pack->header.ackquant);
            crow::release(pack);
        }

        void undelivered_packet(crow_packet *pack)
        {
            nid_t rid;
            crow::hostaddr_view host;

            opposite_address(pack, rid, host);
            node::send_special(rid, host, CROW_NODE_SPECIAL_BUS_ERROR, "",
                               pack->header.qos, pack->header.ackquant);
            crow::release(pack);
        }
    };
} // namespace crow

#endif
