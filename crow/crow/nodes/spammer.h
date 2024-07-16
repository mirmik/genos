/**
    @file
*/

#ifndef CROW_NODE_SPAMMER_H
#define CROW_NODE_SPAMMER_H

#include <chrono>
#include <crow/nodeaddr.h>
#include <crow/proto/node.h>
#include <map>

#include <igris/event/delegate.h>

using namespace std::literals::chrono_literals;

namespace crow
{
    class spammer : public crow::node
    {
        struct record
        {
            std::chrono::time_point<std::chrono::system_clock> last_subscribe =
                {};
        };

        std::map<nodeaddr, record> targets = {};

        std::chrono::milliseconds timeout = 5000ms;
        uint8_t qos = 0;
        uint16_t ackquant = 50;

    public:
        void send(nos::buffer data);
        void send_v(nos::buffer *data, size_t sz);
        void incoming_packet(crow::packet *pack) override;
        int count_of_subscribers();
    };

    class spam_subscriber : public crow::node
    {
        igris::delegate<void, nos::buffer> dlg = {};
        std::vector<uint8_t> addr = {};
        int nid = {};

    public:
        spam_subscriber(igris::delegate<void, nos::buffer> dlg);
        void subscribe(nodeid_t nid,
                       crow::hostaddr_view host,
                       uint8_t qos = 2,
                       uint16_t ackquant = 200);
        void resubscribe(uint8_t qos = 2, uint16_t ackquant = 200);
        void incoming_packet(crow::packet *pack) override;
    };
}

#endif
