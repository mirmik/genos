#include <crow/nodes/pubsub_defs.h>
#include <crow/nodes/requestor_node.h>
#include <igris/util/bug.h>
#include <nos/print.h>

const char *crow::pubsub_type_to_string(PubSubTypes type)
{
    switch (type)
    {
        case PubSubTypes::Consume:
            return "consume";
        case PubSubTypes::Request:
            return "request";
        case PubSubTypes::Subscribe:
            return "subscribe";
        case PubSubTypes::Unsubscribe:
            return "subscribe";
        case PubSubTypes::Subscribe_v2:
            return "subscribe_v2";
        case PubSubTypes::Publish:
            return "publish";
    }
    return "undefined";
}

void crow::requestor_node::incoming_packet(crow::packet *pack)
{
    auto &s = pack->subheader<pubsub_subheader>();
    if (incoming.armed())
    {
        switch (s.type)
        {
            case PubSubTypes::Consume:
            {
                auto &sh = pack->subheader<consume_subheader>();
                incoming(sh.message());
            };
            break;

            default:
            {
                nos::println(pubsub_type_to_string(s.type));
                BUG();
            }
            break;
        }

        crow::release(pack);
    }

    else
    {
        dlist_move(&pack->ulnk, &incoming_list);
        notify_one(0);
    }
}

void crow::requestor_node::async_request(crow::hostaddr_view crowker_addr,
                                         nodeid_t crowker_node,
                                         nos::buffer theme,
                                         nos::buffer reptheme,
                                         nos::buffer data,
                                         uint8_t qos,
                                         uint16_t ackquant,
                                         uint8_t rqos,
                                         uint16_t rackquant)
{
    this->crowker_node = crowker_node;
    this->crowker_addr = crowker_addr;
    this->qos = qos;
    this->rqos = rqos;
    this->ackquant = ackquant;
    this->rackquant = rackquant;
    this->theme = theme.to_string();
    this->reply_theme = reptheme.to_string();
    async_request(data);
}

void crow::requestor_node::async_request(nos::buffer data)
{
    crow::request_subheader sh;

    sh.type = PubSubTypes::Request;
    sh.rqos = rqos;
    sh.rackquant = rackquant;
    sh.thmsize = theme.size();
    sh.datsize = data.size();
    sh.repthmsize = reply_theme.size();

    const nos::buffer iov[] = {{(char *)&sh + sizeof(node_subheader),
                                sizeof(sh) - sizeof(node_subheader)},
                               {theme.data(), theme.size()},
                               {reply_theme.data(), reply_theme.size()},
                               {data.data(), data.size()}};

    node::send_v(crowker_node, crowker_addr, iov, std::size(iov), qos,
                 ackquant);
}

crow::requestor_node::requestor_node(crow::hostaddr_view crowker_addr,
                                     nos::buffer theme,
                                     nos::buffer reptheme)
    : publisher_node(crowker_addr, theme), reply_theme(reptheme)
{
}

crow::requestor_node::requestor_node(crow::hostaddr_view crowker_addr,
                                     nodeid_t crowker_node,
                                     nos::buffer theme,
                                     nos::buffer reptheme)
    : publisher_node(crowker_addr, crowker_node, theme), reply_theme(reptheme)
{
}

void crow::requestor_node::set_reply_theme(nos::buffer reply_theme)
{
    this->reply_theme = reply_theme.to_string();
}

void crow::requestor_node::undelivered_packet(crow::packet *pack)
{
    BUG();
    notify_one(-1);
    crow::release(pack);
}
