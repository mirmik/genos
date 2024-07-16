#include <crow/brocker/crowker_api.h>
#include <crow/brocker/crowker_pubsub_node.h>
#include <crow/nodes/pubsub_defs.h>
#include <igris/util/bug.h>

std::vector<client *> crow::crowker::clients()
{
    std::vector<client *> ret;
    for (auto *client : crowker_implementation::crow_client::clients())
        ret.push_back(client);
    for (auto *client : crowker_implementation::tcp_client::clients())
        ret.push_back(client);
    for (auto *api : apivec)
    {
        for (auto *client : api->get_clients())
            ret.push_back(client);
    }
    return ret;
}
