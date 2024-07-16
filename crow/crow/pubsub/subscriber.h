/** @file */

#ifndef CROW_PUBSUB_SUBSCRIBER_H
#define CROW_PUBSUB_SUBSCRIBER_H

#include "pubsub.h"

namespace crow
{
    class subscriber
    {
    public:
        dlist_head lnk = DLIST_HEAD_INIT(lnk);

    public:
        crow::hostaddr_view addr = {};
        const char *theme = nullptr;
        uint8_t qos = 0;
        uint16_t ackquant = 0;
        uint8_t rqos = 0;
        uint16_t rackquant = 0;

        igris::delegate<void, crow::pubsub_packet_ptr> dlg = {};

    public:
        subscriber() = default;
        subscriber(igris::delegate<void, crow::pubsub_packet_ptr> dlg)
            : dlg(dlg)
        {
        }

        subscriber(const subscriber &) = delete;
        subscriber &operator=(const subscriber &) = delete;

        virtual void newpack_handler(crow::pubsub_packet_ptr pack)
        {
            dlg(pack);
        }

        void init(igris::delegate<void, crow::pubsub_packet_ptr> dlg)
        {
            this->dlg = dlg;
        }

        subscriber(const crow::hostaddr_view &addr,
                   const char *theme,
                   uint8_t qos,
                   uint16_t ackquant,
                   uint8_t rqos,
                   uint16_t rackquant,
                   igris::delegate<void, crow::pubsub_packet_ptr> dlg)
            : addr(addr)
        {
            this->theme = theme;
            this->qos = qos;
            this->ackquant = ackquant;
            this->rqos = rqos;
            this->rackquant = rackquant;
            this->dlg = dlg;

            system_lock();
            dlist_add(&lnk, &pubsub_protocol.subscribers);
            system_unlock();
        }
        virtual ~subscriber() = default;

        void subscribe(const crow::hostaddr_view &addr,
                       const char *theme,
                       uint8_t qos,
                       uint16_t ackquant,
                       uint8_t rqos,
                       uint16_t rackquant
                       // igris::delegate<void, crow::pubsub_packet_ptr> dlg
        )
        {
            this->addr = addr;
            this->theme = theme;
            this->qos = qos;
            this->ackquant = ackquant;
            this->rqos = rqos;
            this->rackquant = rackquant;

            system_lock();
            dlist_add(&lnk, &pubsub_protocol.subscribers);
            system_unlock();

            resubscribe();
        }

        void resubscribe()
        {
            crow::subscribe(addr, theme, qos, ackquant, rqos, rackquant);
        }
    };
} // namespace crow

#endif