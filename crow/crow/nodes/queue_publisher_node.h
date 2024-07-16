#ifndef CROW_QUEUE_PUBLISHER_NODE_H_
#define CROW_QUEUE_PUBLISHER_NODE_H_

#include <crow/nodes/publisher_node.h>
#include <crow/proto/node.h>

#include <deque>
#include <memory>
#include <string>

namespace crow
{
    class queue_publisher_node : public crow::publisher_node
    {
        std::deque<std::shared_ptr<std::string>> queue = {};

      public:
        queue_publisher_node(crow::hostaddr_view addr, int crowker_node,
                             std::string theme, int qos, int ackquant)
            : crow::publisher_node(addr, crowker_node, theme)
        {
        }

        void send_block() {}

        void undelivered_packet(crow::packet *pack) override
        {
            crow::release(pack);
            send_block();
        }

        void delivered_packet(crow::packet *pack) override
        {
            crow::release(pack);
            send_block();
        }
    };
}

#endif