#include <crow/nodes/pubsub_defs.h>
#include <crow/nodes/subscriber_node.h>

void crow::subscriber_node::incoming_packet(crow::packet *pack)
{
    auto &s = pack->subheader<pubsub_subheader>();

    switch (s.type)
    {
        case PubSubTypes::Consume:
        {
            auto &sh = pack->subheader<consume_subheader>();
            incoming_handler(sh.message());
        };
        break;

        default:
            break;
    }

    crow::release(pack);
}

crow::subscriber_node::subscriber_node(function incoming)
    : incoming_handler(incoming)
{
}

void crow::abstract_subscriber_node::init_subscribe(
    crow::hostaddr_view crowker_addr,
    int crowker_node,
    nos::buffer theme,
    uint8_t qos,
    uint16_t ackquant,
    uint8_t rqos,
    uint16_t rackquant)
{
    this->crowker_addr = crowker_addr;
    this->crowker_node = crowker_node;
    this->theme = {theme.data(), theme.size()};
    this->qos = qos;
    this->ackquant = ackquant;
    this->rqos = rqos;
    this->rackquant = rackquant;
}

void crow::abstract_subscriber_node::set_brocker_address(
    crow::hostaddr_view crowker_addr, int crowker_node)
{
    this->crowker_addr = crowker_addr;
    this->crowker_node = crowker_node;
}

void crow::abstract_subscriber_node::subscribe()
{
    crow::subscribe_subheader sh;

    sh.type = PubSubTypes::Subscribe;
    sh.rqos = rqos;
    sh.rackquant = rackquant;
    sh.thmsize = theme.size();

    const nos::buffer iov[] = {{(char *)&sh + sizeof(node_subheader),
                                sizeof(sh) - sizeof(node_subheader)},
                               theme};

    node::send_v(crowker_node, crowker_addr, iov, std::size(iov), qos,
                 ackquant);
}

void crow::abstract_subscriber_node::unsubscribe()
{
    crow::subscribe_subheader sh;

    sh.type = PubSubTypes::Unsubscribe;
    sh.rqos = rqos;
    sh.rackquant = rackquant;
    sh.thmsize = theme.size();

    const nos::buffer iov[] = {{(char *)&sh + sizeof(node_subheader),
                                sizeof(sh) - sizeof(node_subheader)},
                               theme};

    node::send_v(crowker_node, crowker_addr, iov, std::size(iov), qos,
                 ackquant);
}

void crow::abstract_subscriber_node::subscribe_v2(bool updates,
                                                  uint32_t request_latest)
{
    crow::subscribe_subheader_v2 sh;

    sh.type = PubSubTypes::Subscribe_v2;
    sh.rqos = rqos;
    sh.rackquant = rackquant;
    sh.thmsize = theme.size();
    sh.cmd.f.subscribe_on_updates = updates;
    sh.cmd.f.request_latest = request_latest;

    const nos::buffer iov[] = {{(char *)&sh + sizeof(node_subheader),
                                sizeof(sh) - sizeof(node_subheader)},
                               theme};

    node::send_v(crowker_node, crowker_addr, iov, std::size(iov), qos,
                 ackquant);
}
