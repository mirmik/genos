#ifndef CROW_requestor_node_H
#define CROW_requestor_node_H

#include <crow/nodes/publisher_node.h>
#include <crow/proto/node.h>
#include <igris/event/delegate.h>

namespace crow
{
    class requestor_node : public crow::publisher_node
    {
        dlist_head incoming_list = DLIST_HEAD_INIT(incoming_list);
        std::string reply_theme = {};

        int rqos = 0;
        int rackquant = 0;

        igris::delegate<void, nos::buffer> incoming = {};

    public:
        requestor_node() = default;
        requestor_node(igris::delegate<void, nos::buffer> incoming)
        {
            set_async_handle(incoming);
        }
        requestor_node(crow::hostaddr_view crowker_addr,
                       nos::buffer theme,
                       nos::buffer reptheme);
        requestor_node(crow::hostaddr_view crowker_addr,
                       nodeid_t crowker_node,
                       nos::buffer theme,
                       nos::buffer reptheme);

        void async_request(crow::hostaddr_view crowker_addr,
                           nodeid_t crowker_node,
                           nos::buffer theme,
                           nos::buffer reptheme,
                           nos::buffer data,
                           uint8_t qos,
                           uint16_t ackquant,
                           uint8_t rqos,
                           uint16_t rackquant);

        void async_request(nos::buffer data);

        template <class... Args> crow::packet_ptr request(Args &&... data)
        {
            async_request(std::forward<Args>(data)...);
            int sts = waitevent();

            if (sts != 0)
            {
                return crow::packet_ptr(nullptr);
            }

            auto *ptr = dlist_first_entry(&incoming_list, crow::packet, ulnk);
            return crow::packet_ptr(ptr);
        }

        void set_reply_theme(nos::buffer reply_theme);
        requestor_node &
        set_async_handle(igris::delegate<void, nos::buffer> incoming)
        {
            this->incoming = incoming;
            return *this;
        }

        void set_rqos(int _rqos, int _rackquant)
        {
            rqos = _rqos;
            rackquant = _rackquant;
        }

    private:
        void incoming_packet(crow::packet *pack) override;
        void undelivered_packet(crow::packet *pack) override;
    };
}

#endif