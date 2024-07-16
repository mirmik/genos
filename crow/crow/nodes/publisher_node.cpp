#include <crow/nodes/publisher_node.h>
#include <crow/nodes/pubsub_defs.h>

void crow::abstract_publisher_node::publish_v(crow::hostaddr_view addr,
                                              int node,
                                              nos::buffer theme,
                                              nos::buffer *data,
                                              int len,
                                              int qos,
                                              int ackquant)
{
    crow::publish_subheader sh;

    sh.type = PubSubTypes::Publish;
    sh.thmsize = theme.size();
    sh.datsize = 0;
    for (int i = 0; i < len; i++)
        sh.datsize += data[i].size();

    const nos::buffer iov[] = {
        {
            (char *)&sh + sizeof(node_subheader),
            sizeof(sh) - sizeof(node_subheader),
        },
        theme,
    };

    node::send_vv(node, addr, iov, std::size(iov), data, len, qos, ackquant,
                  _async);
}

void crow::abstract_publisher_node::publish(crow::hostaddr_view addr,
                                            int node,
                                            nos::buffer theme,
                                            nos::buffer data,
                                            int qos,
                                            int ackquant)
{
    abstract_publisher_node::publish_v(addr, node, theme, &data, 1, qos,
                                       ackquant);
}

void crow::publisher_node::publish_v(nos::buffer *data, int len)
{
    abstract_publisher_node::publish_v(crowker_addr, crowker_node, theme, data,
                                       len, qos, ackquant);
}

void crow::publisher_node::publish(nos::buffer data)
{
    abstract_publisher_node::publish_v(crowker_addr, crowker_node, theme, &data,
                                       1, qos, ackquant);
}

void crow::publisher_node::set_theme(nos::buffer theme)
{
    this->theme = theme.to_string();
}

void crow::publisher_node::set_address(crow::hostaddr_view addr)
{
    this->crowker_addr = addr;
}

crow::publisher_node::publisher_node(crow::hostaddr_view crowker_addr,
                                     int crowker_node,
                                     nos::buffer theme)
    : crowker_addr(crowker_addr), crowker_node(crowker_node), theme(theme)
{
}

crow::publisher_node::publisher_node(crow::hostaddr_view crowker_addr,
                                     nos::buffer theme)
    : crowker_addr(crowker_addr), crowker_node(CROWKER_SERVICE_BROCKER_NODE_NO),
      theme(theme)
{
}