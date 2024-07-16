#ifndef CROW_NODE_H
#define CROW_NODE_H

#include <crow/packet.h>
#include <crow/proto/protocol.h>
#include <crow/types.h>

#include <igris/binreader.h>
#include <igris/datastruct/dlist.h>
#include <igris/sync/syslock.h>

#define CROW_NODEPACK_COMMON 0
#define CROW_NODEPACK_ERROR 1

#define CROW_NODE_SPECIAL_BUS_ERROR -2

#define CROW_ERRNO_UNREGISTRED_RID 33

namespace crow
{
    crow::packet_ptr node_send(uint16_t sid,
                               uint16_t rid,
                               const crow::hostaddr_view &addr,
                               const nos::buffer data,
                               uint8_t qos,
                               uint16_t ackquant,
                               bool fastsend = CROW_FASTSEND_DEFAULT);

    crow::packet_ptr node_send_special(uint16_t sid,
                                       uint16_t rid,
                                       const crow::hostaddr_view &addr,
                                       uint8_t type,
                                       const nos::buffer data,
                                       uint8_t qos,
                                       uint16_t ackquant,
                                       bool fastsend = CROW_FASTSEND_DEFAULT);

    crow::packet_ptr node_send_v(uint16_t sid,
                                 uint16_t rid,
                                 const crow::hostaddr_view &addr,
                                 const nos::buffer *vec,
                                 size_t veclen,
                                 uint8_t qos,
                                 uint16_t ackquant,
                                 bool fastsend = CROW_FASTSEND_DEFAULT);

    // TODO: replace with annotation
    struct node_subheader
    {
        uint16_t sid;
        uint16_t rid;
        union
        {
            uint8_t flags = 0;

            struct
            {
                uint8_t reserved : 4;
                uint8_t type : 4;
            } f;
        };
    } __attribute__((packed));

    struct node_subheader_annotation
    {
        uint16_t sid;
        uint16_t rid;
        union
        {
            uint8_t flags = 0;

            struct
            {
                uint8_t reserved : 4;
                uint8_t type : 4;
            } f;
        };

        int parse(nos::buffer data)
        {
            igris::binreader reader(data.data());

            if (data.size() < sizeof(node_subheader))
                return -1;

            reader.read_binary(sid);
            reader.read_binary(rid);
            reader.read_binary(flags);

            return 0;
        }
    };

    struct node;
    crow::node *find_node(int id);
    void __link_node(node *srvs, uint16_t id);
    void bind_node_dynamic(node *srvs);

    static auto node_data(crow_packet *pack)
    {
        return nos::buffer(crow_packet_dataptr(pack) + sizeof(node_subheader),
                           crow_packet_datasize(pack) - sizeof(node_subheader));
    }

    class node
    {
    public:
        struct dlist_head lnk = DLIST_HEAD_INIT(lnk); // Список нодов.
        struct dlist_head waitlnk =
            DLIST_HEAD_INIT(waitlnk); // Список ожидающих прихода сообщения.
        uint16_t id = 0;

        virtual void incoming_packet(crow_packet *pack)
        {
            crow::release(pack);
        }

        virtual void undelivered_packet(crow_packet *pack)
        {
            notify_all(-1);
            crow::release(pack);
        }
        int waitevent();
        void notify_one(int future);
        void notify_all(int future);

        virtual const char *typestr()
        {
            return "node";
        }

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

        crow::packet_ptr send(uint16_t rid,
                              const crow::hostaddr_view &raddr,
                              const nos::buffer data,
                              uint8_t qos,
                              uint16_t ackquant,
                              bool fastsend = CROW_FASTSEND_DEFAULT)
        {
            assert(id != 0);
            return crow::node_send(id, rid, raddr, data, qos, ackquant,
                                   fastsend);
        }

        crow::packet_ptr send_special(uint16_t rid,
                                      const crow::hostaddr_view &raddr,
                                      uint8_t type,
                                      const nos::buffer data,
                                      uint8_t qos,
                                      uint16_t ackquant,
                                      bool fastsend = CROW_FASTSEND_DEFAULT)
        {
            assert(id != 0);
            return crow::node_send_special(id, rid, raddr, type, data, qos,
                                           ackquant, fastsend);
        }

        crow::packet_ptr send_v(uint16_t rid,
                                const crow::hostaddr_view &raddr,
                                const nos::buffer *vdat,
                                size_t vlen,
                                uint8_t qos,
                                uint16_t ackquant,
                                bool fastsend = CROW_FASTSEND_DEFAULT)
        {
            assert(id != 0);
            return crow::node_send_v(id, rid, raddr, vdat, vlen, qos, ackquant,
                                     fastsend);
        }

        static nos::buffer message(crow_packet *pack)
        {
            return node_data(pack);
        }

        static node_subheader *subheader(crow_packet *pack)
        {
            return (crow::node_subheader *)crow_packet_dataptr(pack);
        }

        static nid_t sid(crow_packet *pack)
        {
            return subheader(pack)->sid;
        }

        static node_subheader_annotation annotation(crow_packet *pack)
        {
            node_subheader_annotation annot;
            annot.parse(crow_packet_dataptr(pack));
            return annot;
        }

        virtual ~node();
    };

    class node_protocol_cls : public crow::protocol
    {
    private:
        void send_node_error(crow_packet *pack, int errcode);

    public:
        void incoming(crow_packet *pack) override;
        void undelivered(crow_packet *pack) override;

        node_protocol_cls() : protocol(CROW_NODE_PROTOCOL) {}

        static auto sid(crow_packet *pack)
        {
            return ((node_subheader *)(crow_packet_dataptr(pack)))->sid;
        }
        static auto rid(crow_packet *pack)
        {
            return ((node_subheader *)(crow_packet_dataptr(pack)))->rid;
        }

        static auto get_error_code(crow_packet *pack)
        {
            return *(int *)(node_data(pack).data());
        }
    };
    extern node_protocol_cls node_protocol;
    extern struct dlist_head nodes_list;

    class node_packet_ptr : public packet_ptr
    {
    public:
        node_packet_ptr(crow_packet *pack_) : packet_ptr(pack_) {}
        node_packet_ptr(const crow::packet_ptr &oth) : packet_ptr(oth) {}
        node_packet_ptr(crow::packet_ptr &&oth) : packet_ptr(std::move(oth)) {}

        int rid()
        {
            auto *h = node::subheader(pack);
            return h->rid;
        }

        nos::buffer message()
        {
            return node_data(pack);
        }
    };
} // namespace crow

#endif
