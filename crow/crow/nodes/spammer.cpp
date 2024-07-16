#include <crow/nodes/spammer.h>

crow::spam_subscriber::spam_subscriber(igris::delegate<void, nos::buffer> dlg)
    : dlg(dlg)
{
}

void crow::spam_subscriber::subscribe(nodeid_t nid,
                                      crow::hostaddr_view host,
                                      uint8_t qos,
                                      uint16_t ackquant)
{
    this->addr = std::vector<uint8_t>((uint8_t *)host.data(),
                                      (uint8_t *)host.data() + host.size());
    this->nid = nid;
    node::send(nid, host, "", qos, ackquant);
}

void crow::spam_subscriber::resubscribe(uint8_t qos, uint16_t ackquant)
{
    node::send(nid, {addr.data(), addr.size()}, "", qos, ackquant);
}

void crow::spam_subscriber::incoming_packet(crow::packet *pack)
{
    crow::node_packet_ptr npack(pack, this);
    dlg(npack.message());
}

void crow::spammer::send(nos::buffer data)
{
    auto time = std::chrono::system_clock::now();

    std::vector<std::map<nodeaddr, record>::iterator> to_delete;

    auto eit = targets.end();
    auto it = targets.begin();
    for (; it != eit; it++)
    {
        if (time - it->second.last_subscribe > timeout)
        {
            to_delete.push_back(it);
            continue;
        }

        node::send(it->first.nid, it->first.hostaddr(), data, qos, ackquant);
    }

    for (auto it : to_delete)
    {
        targets.erase(it);
    }
}

void crow::spammer::send_v(nos::buffer *data, size_t sz)
{

    auto time = std::chrono::system_clock::now();

    std::vector<std::map<nodeaddr, record>::iterator> to_delete;

    auto eit = targets.end();
    auto it = targets.begin();
    for (; it != eit; it++)
    {
        if (time - it->second.last_subscribe > timeout)
        {
            to_delete.push_back(it);
            continue;
        }

        node::send_v(it->first.nid, it->first.hostaddr(), data, sz, qos,
                     ackquant);
    }

    for (auto it : to_delete)
    {
        targets.erase(it);
    }
}

void crow::spammer::incoming_packet(crow::packet *pack)
{
    crow::node_packet_ptr npack(pack, this);
    auto time = std::chrono::system_clock::now();

    std::vector<uint8_t> addr(pack->addrptr(),
                              pack->addrptr() + pack->addrsize());
    targets[nodeaddr{addr, (nodeid_t)npack.sid()}] = record{time};
}

int crow::spammer::count_of_subscribers()
{
    return targets.size();
}
