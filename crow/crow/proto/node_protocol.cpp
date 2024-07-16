#include <crow/proto/node.h>
#include <crow/proto/node_protocol.h>

void crow::node_protocol_cls::send_node_error(crow::packet *pack, int errcode)
{
    crow::node_subheader sh;

    sh.sid = crow::node_protocol.rid(pack);
    sh.rid = crow::node_protocol.sid(pack);
    sh.u.f.type = CROW_NODEPACK_ERROR;

    const nos::buffer iov[2] = {{(char *)&sh, sizeof(sh)},
                                {(char *)&errcode, sizeof(errcode)}};

    crow::send_v({pack->addrptr(), pack->addrsize()}, iov, 2,
                 CROW_NODE_PROTOCOL, 0, pack->ackquant(), true);
}

void crow::node_protocol_cls::incoming(crow::packet *pack)
{
    auto &sh = pack->subheader<node_subheader>();
    crow::node *srv = nullptr;

    for (crow::node &srvs : crow::nodes_list)
    {
        if (srvs.id == sh.rid)
        {
            srv = &srvs;
            break;
        }
    }

    if (srv == nullptr)
    {
        send_node_error(pack, CROW_ERRNO_UNREGISTRED_RID);
        crow::release(pack);
        return;
    }

    switch (sh.u.f.type)
    {
        case CROW_NODEPACK_COMMON:
            srv->incoming_packet(pack);
            break;

        case CROW_NODEPACK_ERROR:
            srv->notify_one(get_error_code(pack));
            crow::release(pack);
            break;
    }
    return;
}

void crow::node_protocol_cls::undelivered(crow::packet *pack)
{
    crow::node_subheader *sh = (crow::node_subheader *)pack->dataptr();
    for (crow::node &srvs : crow::nodes_list)
    {
        if (srvs.id == sh->sid)
        {
            srvs.undelivered_packet(pack);
            return;
        }
    }
    crow::release(pack);
}

void crow::node_protocol_cls::delivered(crow::packet *pack)
{
    crow::node_subheader *sh = (crow::node_subheader *)pack->dataptr();

    for (auto &srvs : crow::nodes_list)
    {
        if (srvs.id == sh->sid)
        {
            srvs.delivered_packet(pack);
            return;
        }
    }

    crow::release(pack);
}
