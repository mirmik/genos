#ifndef CROW_FUNCTIONAL_H
#define CROW_FUNCTIONAL_H

#include <crow/nodes/subscriber_node.h>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace crow
{
    extern std::list<std::unique_ptr<crow::subscriber_node>> subscribers;

    crow::subscriber_node &subscribe(nos::buffer theme,
                                     std::function<void(nos::buffer)>);

    void publish(nos::buffer theme, nos::buffer data);
}

#endif
