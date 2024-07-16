#include <crow/defs.h>
#include <crow/proto/node.h>
#include <crow/tower.h>

#include <crow/print.h>
#include <igris/sync/syslock.h>
#include <igris/util/numconvert.h>

#include <nos/print.h>

igris::dlist<crow::node, &crow::node::lnk> crow::nodes_list;
crow::node_protocol_cls crow::node_protocol;

crow::packet_ptr crow::node::send(nodeid_t sid,
                                  nodeid_t rid,
                                  const crow::hostaddr_view &addr,
                                  const nos::buffer data,
                                  uint8_t qos,
                                  uint16_t ackquant,
                                  bool async)
{
    crow::node_subheader sh;
    sh.sid = sid;
    sh.rid = rid;
    sh.u.f.type = CROW_NODEPACK_COMMON;

    const nos::buffer iov[2] = {{(char *)&sh, sizeof(sh)},
                                {(char *)data.data(), data.size()}};

    return crow::send_v(addr, iov, 2, CROW_NODE_PROTOCOL, qos, ackquant, async);
}

crow::packet_ptr crow::node::send_v(nodeid_t sid,
                                    nodeid_t rid,
                                    const crow::hostaddr_view &addr,
                                    const nos::buffer *vec,
                                    size_t veclen,
                                    uint8_t qos,
                                    uint16_t ackquant,
                                    bool async)
{
    crow::node_subheader sh;
    sh.sid = sid;
    sh.rid = rid;
    sh.u.f.type = CROW_NODEPACK_COMMON;

    const nos::buffer iov[1] = {{(char *)&sh, sizeof(sh)}};

    return crow::send_vv(addr, iov, 1, vec, veclen, CROW_NODE_PROTOCOL, qos,
                         ackquant, async);
}

crow::packet_ptr crow::node::send_vv(nodeid_t sid,
                                     nodeid_t rid,
                                     const crow::hostaddr_view &addr,
                                     const nos::buffer *vec1,
                                     size_t veclen1,
                                     const nos::buffer *vec2,
                                     size_t veclen2,
                                     uint8_t qos,
                                     uint16_t ackquant,
                                     bool async)
{
    crow::node_subheader sh;
    sh.sid = sid;
    sh.rid = rid;
    sh.u.f.type = CROW_NODEPACK_COMMON;

    const nos::buffer iov[1] = {{(char *)&sh, sizeof(sh)}};

    return crow::send_vvv(addr, iov, 1, vec1, veclen1, vec2, veclen2,
                          CROW_NODE_PROTOCOL, qos, ackquant, async);
}

void crow::__link_node(crow::node *srv, uint16_t id)
{
    srv->id = id;
    nodes_list.move_back(*srv);
}

crow::node *crow::find_node(size_t id)
{
    // TODO: переделать на хештаблицу
    for (auto &node : nodes_list)
    {
        if (node.id == id)
            return &node;
    }

    return nullptr;
}

void crow::bind_node_dynamic(crow::node *srv)
{
    // Динамические порты располагаются в верхнем полупространстве.
    static nodeid_t counter = 1 << (sizeof(nodeid_t) * 8 - 1);

    system_lock();
    do
    {
        counter++;
        if (counter == 0)
            counter = (1 << (sizeof(nodeid_t) * 8 - 1));
    } while (crow::find_node(counter) != nullptr);

    __link_node(srv, counter);
    system_unlock();
}

crow::node::~node()
{
    system_lock();
    if (!waitlnk.empty())
        notify_all(-1);
    lnk.unlink();
    system_unlock();
}

crow::alived_object::~alived_object()
{
    system_lock();
    keepalive_timer.unplan();
    system_unlock();
}

void crow::node_keepalive_timer::execute()
{
    alived_object &n = *mcast_out(this, alived_object, keepalive_timer);
    n.keepalive_handle();
}