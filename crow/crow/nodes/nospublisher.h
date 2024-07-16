#ifndef CROW_NODES_NOSPUBLISHER_H
#define CROW_NODES_NOSPUBLISHER_H

#include <crow/nodes/publisher_node.h>
#include <nos/io/ostream.h>

namespace crow
{
    class nospublisher : public crow::publisher_node, public nos::ostream
    {
    public:
        nospublisher() = default;
        nospublisher(crow::hostaddr_view crowker_addr,
                     int crowker_node,
                     nos::buffer theme)
            : publisher_node(crowker_addr, crowker_node, theme)
        {
        }

        nospublisher(crow::hostaddr_view crowker_addr, nos::buffer theme)
            : nospublisher(crowker_addr, CROWKER_SERVICE_BROCKER_NODE_NO, theme)
        {
        }

        nospublisher(const nospublisher &) = default;

        nos::expected<size_t, nos::output_error> write(const void *ptr,
                                                       size_t sz) override
        {
            crow::publisher_node::publish({ptr, sz});
            return sz;
        }
    };
}

#endif