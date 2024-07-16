#ifndef CROW_BEAM_H
#define CROW_BEAM_H

#include <crow/proto/node.h>

namespace crow
{
    class beam : public crow::node, public crow::alived_object
    {
        std::string client_name;
        crow::hostaddr crowker;
        nodeid_t nodeno = CROWKER_BEAMSOCKET_BROCKER_NODE_NO;

    public:
        void set_crowker_address(const crow::hostaddr &addr)
        {
            this->crowker = addr;
        }

        void set_client_name(const std::string &client_name)
        {
            this->client_name = client_name;
        }

        void keepalive_handle() override
        {
            node::send(nodeno, crowker, client_name, 0, 0);
        }

        void incoming_packet(crow::packet *pack) override
        {
            crow::release(pack);
        }
    };
}

#endif