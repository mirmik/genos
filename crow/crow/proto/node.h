#ifndef CROW_NODE_H
#define CROW_NODE_H

#include <crow/keepalive.h>
#include <crow/packet.h>
#include <crow/packet_ptr.h>
#include <crow/proto/node_protocol.h>
#include <crow/proto/protocol.h>
#include <crow/tower.h>
#include <igris/binreader.h>
#include <igris/container/dlist.h>
#include <igris/sync/syslock.h>
#include <stdint.h>

#define CROW_NODEPACK_COMMON 0
#define CROW_NODEPACK_ERROR 1
#define CROW_ERRNO_UNREGISTRED_RID 33

namespace crow
{
    class node;

    crow::node *find_node(size_t id);
    void __link_node(node *srvs, uint16_t id);
    void bind_node_dynamic(node *srvs);

    class node_keepalive_timer : public igris::timer_head
    {
        void execute() override;
    };

    class node
    {
    public:
        igris::dlist_node lnk = {}; // Список нодов.
        igris::dlist_base waitlnk = {}; // Список ожидающих прихода сообщения.
        nodeid_t id = 0;

    public:
        node() = default;
        node(const node &) = delete;
        node(node &&) = delete;

        int waitevent();
        void notify_one(intptr_t future);
        void notify_all(intptr_t future);

        node &bind(int addr)
        {
            system_lock();
            __link_node(this, addr);
            system_unlock();
            return *this;
        };

        node &bind()
        {
            bind_node_dynamic(this);
            return *this;
        };

        crow::packet_ptr send(nodeid_t rid,
                              const crow::hostaddr_view &raddr,
                              const nos::buffer data,
                              uint8_t qos = CROW_DEFAULT_QOS,
                              uint16_t ackquant = CROW_DEFAULT_ACKQUANT,
                              bool async = false)
        {
            if (id == 0)
                bind();
            return crow::node::send(id, rid, raddr, data, qos, ackquant, async);
        }

        crow::packet_ptr send_v(nodeid_t rid,
                                const crow::hostaddr_view &raddr,
                                const nos::buffer *vdat,
                                size_t vlen,
                                uint8_t qos,
                                uint16_t ackquant,
                                bool async = false)
        {
            if (id == 0)
                bind();
            return crow::node::send_v(id, rid, raddr, vdat, vlen, qos, ackquant,
                                      async);
        }

        crow::packet_ptr send_vv(nodeid_t rid,
                                 const crow::hostaddr_view &raddr,
                                 const nos::buffer *vdat1,
                                 size_t vlen1,
                                 const nos::buffer *vdat2,
                                 size_t vlen2,
                                 uint8_t qos,
                                 uint16_t ackquant,
                                 bool async = false)
        {
            if (id == 0)
                bind();
            return crow::node::send_vv(id, rid, raddr, vdat1, vlen1, vdat2,
                                       vlen2, qos, ackquant, async);
        }

        static crow::packet_ptr send(nodeid_t sid,
                                     nodeid_t rid,
                                     const crow::hostaddr_view &addr,
                                     const nos::buffer data,
                                     uint8_t qos,
                                     uint16_t ackquant,
                                     bool async = false);

        static crow::packet_ptr send_v(nodeid_t sid,
                                       nodeid_t rid,
                                       const crow::hostaddr_view &addr,
                                       const nos::buffer *vec,
                                       size_t veclen,
                                       uint8_t qos,
                                       uint16_t ackquant,
                                       bool async = false);

        static crow::packet_ptr send_vv(nodeid_t sid,
                                        nodeid_t rid,
                                        const crow::hostaddr_view &addr,
                                        const nos::buffer *vec1,
                                        size_t veclen1,
                                        const nos::buffer *vec2,
                                        size_t veclen2,
                                        uint8_t qos,
                                        uint16_t ackquant,
                                        bool async = false);

        static crow::node_subheader *subheader(crow::packet *pack)
        {
            return (crow::node_subheader *)pack->dataptr();
        }

        virtual ~node();

    private:
        virtual void incoming_packet(crow::packet *pack) = 0;

        virtual void undelivered_packet(crow::packet *pack)
        {
            notify_all(-1);
            crow::release(pack);
        }

        virtual void delivered_packet(crow::packet *pack)
        {
            crow::release(pack);
        }

        friend class node_protocol_cls;
    };

    class alived_object
    {
    public:
        node_keepalive_timer keepalive_timer = {};

        virtual void keepalive_handle() {}

        void install_keepalive(int64_t interval, bool immediate_call = true)
        {
            crow::keepalive_timer_manager.plan(keepalive_timer, igris::millis(),
                                               interval);
            crow::unsleep();
            if (immediate_call)
            {
                keepalive_handle();
            }
        };

        virtual ~alived_object();
    };

    class node_packet_ptr : public packet_ptr
    {
        node *_node = nullptr;

    public:
        node_packet_ptr(crow::packet *pack_, node *node_)
            : packet_ptr(pack_), _node(node_)
        {
        }

        node_packet_ptr(const crow::packet_ptr &oth, node *node_)
            : packet_ptr(oth), _node(node_)
        {
        }

        node_packet_ptr(crow::packet_ptr &&oth, node *node_)
            : packet_ptr(std::move(oth)), _node(node_)
        {
        }

        node_packet_ptr(std::nullptr_t) : packet_ptr(nullptr), _node(nullptr) {}
        node_packet_ptr(const node_packet_ptr &) = default;
        node_packet_ptr &operator=(const node_packet_ptr &) = default;

        int rid()
        {
            auto *h = node::subheader(pack);
            return h->rid;
        }

        int sid()
        {
            auto *h = node::subheader(pack);
            return h->sid;
        }

        nos::buffer message()
        {
            return node_data(pack);
        }

        void reply(nos::buffer rep)
        {
            _node->send(sid(), pack->addr(), rep, pack->quality(),
                        pack->ackquant());
        }
    };

    extern igris::dlist<crow::node, &crow::node::lnk> nodes_list;
} // namespace crow

#endif
