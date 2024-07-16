#ifndef CROW_CHANNEL_H
#define CROW_CHANNEL_H

#include <crow/proto/node.h>

#include <igris/buffer.h>
#include <igris/event/delegate.h>
#include <igris/sync/syslock.h>

#define CROW_CHANNEL_INIT 0
#define CROW_CHANNEL_WAIT_HANDSHAKE_REQUEST 1
#define CROW_CHANNEL_WAIT_HANDSHAKE_ANSWER 2
#define CROW_CHANNEL_CONNECTED 3
#define CROW_CHANNEL_DISCONNECTED 4

#define CROW_CHANNEL_ERR_NOCONNECT -1

namespace crow
{
    enum class Frame : uint8_t
    {
        HANDSHAKE_REQUEST = 0,
        HANDSHAKE_ANSWER = 1,
        DATA = 2,
        REFUSE = 3,
    };

    class channel;

    class channel : public crow::node
    {
    public:
        using incoming_handler_t = void (*)(crow::channel *, crow::packet *);

        dlist_head lnk = DLIST_HEAD_INIT(lnk);
        nodeid_t rid = 0;
        void *raddr_ptr = nullptr;
        size_t raddr_len = 0;
        size_t raddr_cap = 0;
        uint8_t qos = 0;
        uint16_t ackquant = 0;
        uint16_t fid = 0;
        union _u
        {
            uint8_t flags = 0;
            struct _f
            {
                uint8_t _state : 4;
                uint8_t dynamic_addr : 1;
                uint8_t naive_listener : 1;
            } f;
        } u = {};
        incoming_handler_t incoming_handler = {};

        channel() : lnk(DLIST_HEAD_INIT(lnk)){};
        channel(incoming_handler_t incoming_handler)
            : lnk(DLIST_HEAD_INIT(lnk)), incoming_handler(incoming_handler)
        {
        }

        channel(const channel &) = delete;
        channel &operator=(const channel &) = delete;

        void init(int id, incoming_handler_t incoming_handler)
        {
            this->incoming_handler = incoming_handler;
            this->bind(id);
        }

        void set_addr_buffer(char *buf, size_t sz)
        {
            raddr_ptr = buf;
            raddr_cap = sz;
        }

        void naive_listener_mode(bool en)
        {
            u.f.naive_listener = en;
        }
        uint8_t state()
        {
            return (uint8_t)u.f._state;
        }

        void incoming_packet(crow::packet *pack) override;
        void incoming_data_packet(crow::packet *pack);

        void undelivered_packet(crow::packet *pack) override;

        void handshake(const uint8_t *raddr,
                       uint16_t rlen,
                       nodeid_t rid,
                       uint8_t qos = 2,
                       uint16_t ackquant = 200);
        void send_handshake_answer();

        void wait_handshake_request()
        {
            u.f._state = CROW_CHANNEL_WAIT_HANDSHAKE_REQUEST;
        }

        int send(const char *data, size_t size);

        static nos::buffer getdata(crow::packet *pack);

        //////////////////SYNC API/////////////////////////
        int connect(const uint8_t *raddr,
                    uint16_t rlen,
                    nodeid_t rid,
                    uint8_t qos = 2,
                    uint16_t ackquant = 200);

        int syncrecv(crow::packet **ppack);
    };

    struct subheader_channel
    {
        uint16_t frame_id;
        Frame ftype;
    } __attribute__((packed));

    struct subheader_handshake
    {
        uint8_t qos;
        uint16_t ackquant;
    } __attribute__((packed));

    static inline subheader_channel *get_subheader_channel(crow::packet *pack)
    {
        return (subheader_channel *)(pack->dataptr() +
                                     sizeof(crow::node_subheader));
    }

    static inline subheader_handshake *
    get_subheader_handshake(crow::packet *pack)
    {
        return (subheader_handshake *)(pack->dataptr() +
                                       sizeof(crow::node_subheader) +
                                       sizeof(crow::subheader_channel));
    }
} // namespace crow

#endif
