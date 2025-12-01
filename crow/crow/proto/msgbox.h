#ifndef CROW_MSGBOX_H
#define CROW_MSGBOX_H

#include <crow/proto/node.h>
#include <igris/datastruct/dlist.h>
#include <igris/sync/semaphore.h>
#include <igris/sync/syslock.h>

#include <vector>

#define CROW_MSGBOX_STATE_NONE 0
#define CROW_MSGBOX_STATE_SEND 1
#define CROW_MSGBOX_STATE_RECEIVE 2

namespace crow
{
    class msgbox : public crow::node
    {
        igris_sem_t message_lock = {};
        struct dlist_head messages = DLIST_HEAD_INIT(messages);

    public:
        msgbox();

        crow::node_packet_ptr query(nodeid_t rid,
                                    const crow::hostaddr_view &addr,
                                    const nos::buffer data,
                                    uint8_t qos,
                                    uint16_t ackquant);

        crow::node_packet_ptr receive();

        crow::packet_ptr reply(crow::node_packet_ptr msg,
                               nos::buffer data,
                               uint8_t qos,
                               uint16_t ackquant);

        void incoming_packet(crow::packet *pack) override;

        void undelivered_packet(crow::packet *pack) override;

        ~msgbox() override;
    };
} // namespace crow

#endif
