/** @file */

#ifndef CROW_PUBSUB_H
#define CROW_PUBSUB_H

#include <assert.h>
#include <crow/proto/protocol.h>
#include <crow/tower.h>

#include <igris/buffer.h>
#include <igris/event/delegate.h>
#include <igris/sync/syslock.h>

#include <igris/buffer.h>

struct crow_theme;

#define SUBSCRIBE 0
#define PUBLISH 1
#define MESSAGE 2
//#define GETLAST 3

typedef struct crow_subheader_pubsub
{
    uint8_t type;
    uint8_t thmsz;
} __attribute__((packed)) crow_subheader_pubsub_t;

typedef struct crow_subheader_pubsub_data
{
    uint16_t datsz;
} __attribute__((packed)) crow_subheader_pubsub_data_t;

typedef struct crow_subheader_pubsub_control
{
    uint8_t qos;
    uint16_t ackquant;
} __attribute__((packed)) crow_subheader_pubsub_control_t;

namespace crow
{
    class pubsub_protocol_cls : public crow::protocol
    {
    public:
        struct dlist_head subscribers = DLIST_HEAD_INIT(subscribers);
        void (*incoming_handler)(packet *);
        void (*undelivered_handler)(packet *);

    public:
        pubsub_protocol_cls() : protocol(CROW_PUBSUB_PROTOCOL) {}

        void incoming(crow_packet *pack) override;
        void undelivered(crow_packet *pack) override;

        static void start_resubscribe_thread(int millis);
        void resubscribe_all();

        static void enable_crowker_subsystem();
        static pubsub_protocol_cls &instance();
    };
    extern pubsub_protocol_cls pubsub_protocol;

    crow::packet_ptr publish(const crow::hostaddr_view &addr,
                             const nos::buffer theme,
                             const nos::buffer data,
                             uint8_t qos,
                             uint16_t acktime,
                             uint8_t type = PUBLISH);

    crow::packet_ptr publish_v(const crow::hostaddr_view &addr,
                               const nos::buffer theme,
                               const nos::buffer *vec,
                               int vecsz,
                               uint8_t qos,
                               uint16_t acktime);

    void subscribe(const crow::hostaddr_view &addr,
                   nos::buffer theme,
                   uint8_t qo0,
                   uint16_t acktime,
                   uint8_t rqos,
                   uint16_t racktime);

    static inline crow_subheader_pubsub_t *
    get_subheader_pubsub(crow_packet *pack)
    {
        return (crow_subheader_pubsub_t *)crow_packet_dataptr(pack);
    }

    static inline crow_subheader_pubsub_data_t *
    get_subheader_pubsub_data(crow_packet *pack)
    {
        return (
            crow_subheader_pubsub_data_t *)(crow_packet_dataptr(pack) +
                                            sizeof(crow_subheader_pubsub_t));
    }

    static inline crow_subheader_pubsub_control_t *
    get_subheader_pubsub_control(crow_packet *pack)
    {
        return (
            crow_subheader_pubsub_control_t *)(crow_packet_dataptr(pack) +
                                               sizeof(crow_subheader_pubsub_t));
    }

    static inline char *packet_pubsub_thmptr(struct crow_packet *pack)
    {
        return crow_packet_dataptr(pack) + sizeof(crow_subheader_pubsub_t) +
               sizeof(crow_subheader_pubsub_data_t);
    }

    static inline char *packet_pubsub_datptr(struct crow_packet *pack)
    {
        return crow_packet_pubsub_thmptr(pack) +
               get_subheader_pubsub(pack)->thmsz;
    }

    namespace pubsub
    {
        static inline nos::buffer get_theme(crow_packet *pack)
        {
            struct crow_subheader_pubsub *shps = get_subheader_pubsub(pack);
            return nos::buffer(crow_packet_pubsub_thmptr(pack), shps->thmsz);
        }

        static inline nos::buffer get_data(crow_packet *pack)
        {
            assert(pack->header.f.type == CROW_PUBSUB_PROTOCOL);
            struct crow_subheader_pubsub_data *shps_d =
                get_subheader_pubsub_data(pack);

            return nos::buffer(crow_packet_pubsub_datptr(pack), shps_d->datsz);
        }
    } // namespace pubsub

    class pubsub_packet_ptr : public packet_ptr
    {
    public:
        pubsub_packet_ptr(crow_packet *pack) : packet_ptr(pack) {}

        pubsub_packet_ptr(const crow::packet_ptr &oth) : packet_ptr(oth.get())
        {
        }

        nos::buffer theme()
        {
            return pubsub::get_theme(pack);
        }
        nos::buffer data()
        {
            return pubsub::get_data(pack);
        }
        nos::buffer message()
        {
            return pubsub::get_data(pack);
        }
    };
} // namespace crow

#endif
