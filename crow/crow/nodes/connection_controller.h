#ifndef CROW_CONNECTION_CONTROLLER_H
#define CROW_CONNECTION_CONTROLLER_H

#include <crow/nodes/node.h>

namespace crow
{
    class connection_controller : public node
    {
        crow::hostaddr addr;

    public:
        connection_controller(const crow::hostaddr &) = default;

        void on_connect() override {}

        void on_disconnect() override {}
    };
} // namespace crow

#endif