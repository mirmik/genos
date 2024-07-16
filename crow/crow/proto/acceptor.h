#ifndef CROW_ACCEPTOR_H
#define CROW_ACCEPTOR_H

#include <crow/proto/channel.h>
#include <crow/proto/node.h>

namespace crow
{

    struct acceptor : public crow::node
    {
        igris::delegate<crow::channel *> init_channel;

        acceptor() = default;
        acceptor(igris::delegate<crow::channel *> init_channel)
            : init_channel(init_channel)
        {
        }

        void init(int id, igris::delegate<crow::channel *> init_channel)
        {
            this->init_channel = init_channel;
            bind(id);
        }

        void incoming_packet(crow::packet *pack) override;
        void undelivered_packet(crow::packet *pack) override;
    };

    __BEGIN_DECLS

    static inline acceptor *
    create_acceptor(uint16_t port, igris::delegate<crow::channel *> dlg)
    {
        auto asrv = new crow::acceptor(dlg);
        asrv->bind(port);
        return asrv;
    }

    uint16_t dynport();

    __END_DECLS

} // namespace crow

#endif
