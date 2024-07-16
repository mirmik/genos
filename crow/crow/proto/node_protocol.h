#ifndef CROW_NODE_PROTOCOL_H
#define CROW_NODE_PROTOCOL_H

#include <crow/proto/protocol.h>
#include <igris/container/dlist.h>

namespace crow
{
    using nodeid_t = uint16_t;

    struct node_subheader
    {
        nodeid_t sid = 0;
        nodeid_t rid = 0;
        union _u
        {
            uint8_t flags = 0;
            struct _f
            {
                uint8_t reserved : 4;
                uint8_t type : 4;
            } f;
        } u = {};
    } __attribute__((packed));

    static auto node_data(crow::packet *pack)
    {
        return nos::buffer(pack->dataptr() + sizeof(node_subheader),
                           pack->datasize() - sizeof(node_subheader));
    }

    class node_protocol_cls : public crow::protocol
    {
    private:
        void send_node_error(crow::packet *pack, int errcode);

    public:
        void incoming(crow::packet *pack);
        void undelivered(crow::packet *pack);
        void delivered(crow::packet *pack);

        node_protocol_cls() /*: protocol(CROW_NODE_PROTOCOL)*/ {}

        static auto sid(crow::packet *pack)
        {
            return ((node_subheader *)(pack->dataptr()))->sid;
        }
        static auto rid(crow::packet *pack)
        {
            return ((node_subheader *)(pack->dataptr()))->rid;
        }

        static auto get_error_code(crow::packet *pack)
        {
            return *(int *)(node_data(pack).data());
        }
    };
    extern node_protocol_cls node_protocol;
}

#endif