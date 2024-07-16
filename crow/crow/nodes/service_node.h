#ifndef CROW_SERVICE_NODE_H
#define CROW_SERVICE_NODE_H

#include <crow/nodes/subscriber_node.h>

namespace crow
{
    class service_node : public crow::abstract_subscriber_node
    {
        using delegate = igris::delegate<void, char *, int, service_node &>;
        delegate dlg = {};
        crow::packet *curpack = nullptr;

    public:
        service_node() = default;
        service_node(const delegate &dlg) : dlg(dlg) {}
        service_node(const crow::hostaddr_view &addr,
                     const nos::buffer &theme,
                     const delegate &dlg)
            : abstract_subscriber_node(addr, theme), dlg(dlg)
        {
            set_theme(theme);
        }

        service_node(const service_node &) = delete;
        service_node &operator=(const service_node &) = delete;

        void set_handle(const delegate &dlg)
        {
            this->dlg = dlg;
        }

        void init(const delegate &dlg)
        {
            this->dlg = dlg;
        }

        void init(const crow::hostaddr_view &addr, const nos::buffer &theme)
        {
            abstract_subscriber_node::init_subscribe(addr, theme, 2, 50, 2, 50);
        }

        void reply(const char *data, size_t size);
        void reply(nos::buffer buf)
        {
            reply(buf.data(), buf.size());
        }

    private:
        void incoming_packet(crow::packet *) override;
    };
}

#endif