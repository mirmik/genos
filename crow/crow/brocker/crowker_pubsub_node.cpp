#include <crow/brocker/crowker_api.h>
#include <crow/brocker/crowker_pubsub_node.h>
#include <crow/nodes/pubsub_defs.h>

#include <igris/util/bug.h>
#include <nos/fprint.h>

void crow::crowker_pubsub_node::incoming_packet(crow::packet *pack)
{
    auto &s = pack->subheader<pubsub_subheader>();

    switch (s.type)
    {
        case PubSubTypes::Publish:
        {
            auto &sh = pack->subheader<publish_subheader>();
            auto message = sh.message();
            publish_to_theme(sh.theme(), std::make_shared<std::string>(
                                             message.data(), message.size()));
        };
        break;

        case PubSubTypes::Subscribe:
        {
            auto &sh = pack->subheader<subscribe_subheader>();
            subscribe_on_theme(pack->addr(), sh.sid, sh.theme(), sh.rqos,
                               sh.rackquant);
        };
        break;

        case PubSubTypes::Subscribe_v2:
        {
            auto &sh = pack->subheader<subscribe_subheader_v2>();
            subscribe_on_theme_v2(pack->addr(), sh.sid, sh.theme(), sh.rqos,
                                  sh.rackquant, sh.cmd.f.subscribe_on_updates,
                                  sh.cmd.f.request_latest);
        };
        break;

        case PubSubTypes::Request:
        {
            auto &sh = pack->subheader<request_subheader>();
            subscribe_on_theme(pack->addr(), sh.sid, sh.reply_theme(), sh.rqos,
                               sh.rackquant);

            uint8_t len = sh.reply_theme().size();
            auto msg = std::make_shared<std::string>(
                nos::format("{}{}{}", nos::buffer((char *)&len, 1),
                            sh.reply_theme(), sh.message()));
            publish_to_theme(sh.theme(), msg);
        };
        break;

        case PubSubTypes::Consume:
        {
        }
        break;

        case PubSubTypes::Unsubscribe:
        {
            auto &sh = pack->subheader<subscribe_subheader>();
            unsubscribe_from_theme(pack->addr(), sh.sid, sh.theme());
        }
        break;
    }
    crow::release(pack);
}

void crow::crowker_pubsub_node::undelivered_packet(crow::packet *pack)
{
    auto node = crow::node_protocol_cls::rid(pack);
    undelivered_packet_handle(pack->addr(), node);
    crow::release(pack);
}

void crow::crowker_pubsub_node::subscribe_on_theme(crow::hostaddr_view view,
                                                   int nid,
                                                   nos::buffer theme,
                                                   uint8_t rqos,
                                                   uint16_t rackquant)
{
    subscribe_on_theme_v2(view, nid, theme, rqos, rackquant, true, 0);
}

void crow::crowker_pubsub_node::subscribe_on_theme_v2(crow::hostaddr_view view,
                                                      int nid,
                                                      nos::buffer theme,
                                                      uint8_t rqos,
                                                      uint16_t rackquant,
                                                      bool subscribe_to_updates,
                                                      uint32_t count_of_latest)
{
    crowker_implementation::options opt{rqos, rackquant};

    auto key = std::make_pair(view, nid);
    auto &client = clients[key];
    client.set_confirmed(rqos != 0);

    client.api = this;
    client.addr = view;
    client.node = nid;
    client.crowker_node = this;

    crow::crowker::instance()->send_latest(theme.to_string(), &client,
                                           count_of_latest);

    if (subscribe_to_updates)
        crow::crowker::instance()->subscribe(theme.to_string(), &client, opt);
}

void crow::crowker_pubsub_node::unsubscribe_from_theme(crow::hostaddr_view view,
                                                       int nid,
                                                       nos::buffer theme)
{
    (void)view;
    (void)nid;
    (void)theme;
    // TODO: implement
}

void crow::crowker_pubsub_node::undelivered_packet_handle(
    crow::hostaddr_view addr, int node)
{
    auto addrstr = addr.to_string();
    nos::log::info("undelivered_packet_handle addr:{} node:{}", addrstr, node);
    auto &client = clients.at(std::make_pair(addr, node));
    client.detach_from_themes();
}

void crow::crowker_pubsub_node::client_beam(crow::hostaddr_view view,
                                            int nid,
                                            nos::buffer name)
{
    auto key = std::make_pair(view, nid);
    auto &client = clients[key];
    client.set_name(name);
}

crow::crowker_pubsub_node::node_client::~node_client() {}

void crow::crowker_pubsub_node::node_client::publish(
    const std::string &theme,
    const std::string &data,
    crowker_implementation::options opts)
{
    crow::consume_subheader sh;

    sh.type = PubSubTypes::Consume;
    sh.thmsize = theme.size();
    sh.datsize = data.size();

    const nos::buffer iov[] = {
        {(char *)&sh + sizeof(node_subheader),
         sizeof(sh) - sizeof(node_subheader)},
        theme,
        data,
    };

    crowker_node->send_v(node, addr, iov, std::size(iov), opts.qos,
                         opts.ackquant);
}

std::vector<crowker_implementation::client *>
crow::crowker_pubsub_node::get_clients()
{
    std::vector<crowker_implementation::client *> ret;
    for (auto &pair : clients)
        ret.push_back(&pair.second);
    return ret;
}
