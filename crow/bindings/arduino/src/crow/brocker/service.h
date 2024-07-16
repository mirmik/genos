/** @file */

#ifndef CROW_BROCKER_SERVICE_H
#define CROW_BROCKER_SERVICE_H

#include <crow/proto/node.h>
#include <crow/proto/request.h>
#include <crow/types.h>

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <igris/util/bug.h>

#define CROW_SERVICE_ONESHOOT 0
#define CROW_SERVICE_NODE_RETRANS 1
#define CROW_SERVICE_CHANNEL_RETRANS 2
#define CROW_SERVICE_CLOSE 3

#define CROW_SERVICE_REGISTER_NODE 8
#define CROW_SERVICE_REGISTER_CHANNEL 9

namespace crow
{
    struct nodeaddr_t
    {
        std::vector<uint8_t> naddr;
        nid_t nid;
    };

    enum service_type_e : uint8_t
    {
        CROWKER_SERVICE_TYPE_NODE = 0,
        CROWKER_SERVICE_TYPE_CHANNEL = 1,
    };

    class crowker_service_node;

    void async_request_callback(void *arg, int sts, crow_packet *pack);

    class node_retransler;
    class channel_retransler;

    struct service_record
    {
        nodeaddr_t naddr;
        service_type_e srvtype;

        std::unordered_set<crow::oneshoot_async_requester *> requesters;
        std::unordered_set<crow::node_retransler *> node_retranslers;
        std::unordered_set<crow::channel_retransler *> channel_retranslers;
    };

    class crowker_service_control_node_cls : public crow::node
    {
        // using register_record_t = std::pair<nodeaddr_t, std::string name>;

        std::unordered_map<std::string, service_record> services;

        // std::vector<uint8_t> host;
        // nodeid_t             nid;
        // std::unordered_set<crow::hostaddr_view *>;

        // public:
        //	crowker_service_control_node_cls() = default;

      protected:
        void incoming_packet(crow_packet *pack) override
        {
            dprln("CONTROL NODE INCOMMING");

            auto nodemsg = node::message(pack);
            auto saddr = pack->addr();
            auto sid = node::sid(pack);

            igris::binreader reader(nodemsg.data());

            uint8_t cmd;
            uint8_t namelen;
            char *name;

            reader.read_binary(cmd);
            reader.read_binary(namelen);
            reader.bind_buffer(name, namelen);

            if (cmd & 0b1000)
            {
                service_record &srv_addr = services[name];

                switch (cmd)
                {
                case CROW_SERVICE_ONESHOOT:
                {
                    uint16_t datalen;
                    char *data;

                    std::vector<uint8_t> addr(saddr.begin(), saddr.end());

                    reader.read_binary(datalen);
                    reader.bind_buffer(data, datalen);

                    crow::async_request(
                        async_request_callback, &srv_addr, srv_addr.naddr.nid,
                        srv_addr.naddr.naddr, node::message(pack), 2,
                        pack->ackquant());
                }
                break;

                case CROW_SERVICE_NODE_RETRANS:
                    BUG();
                    break;
                case CROW_SERVICE_CHANNEL_RETRANS:
                    BUG();
                    break;
                case CROW_SERVICE_CLOSE:
                    BUG();
                    break;
                }
            }

            else
            {
                switch (cmd)
                {
                case CROW_SERVICE_REGISTER_NODE:
                {
                    std::vector<uint8_t> addr(saddr.begin(), saddr.end());

                    services.insert_or_assign(
                        std::string(name, namelen),
                        service_record{{addr, sid}, CROWKER_SERVICE_TYPE_NODE});
                }
                break;

                case CROW_SERVICE_REGISTER_CHANNEL:
                    BUG();
                    break;
                }
            }
            crow::release(pack);
        }

        void undelivered_packet(crow_packet *pack) override
        {
            dprln("CONTROL_NODE_UNDELIVERED");
            crow::release(pack);
        }
    };

    crowker_service_control_node_cls *crowker_service_control_node();
} // namespace crow

#endif
