/** @file */

#ifndef CROW_GATEWAY_H
#define CROW_GATEWAY_H

#include <crow/packet.h>
#include <igris/datastruct/dlist.h>
#include <igris/sync/syslock.h>

struct crow_gateway;

struct crow_gateway_operations
{
    void (*send)(struct crow_gateway *, crow::packet *);
    void (*deinit)(struct crow_gateway *);

    // selector support operations
    int (*get_fds)(struct crow_gateway *, int *fds);
    void (*onestep)(struct crow_gateway *);
};

struct crow_gateway
{
    struct dlist_head lnk; ///< встроенное поле списка.
    const struct crow_gateway_operations *ops;

    uint8_t id; ///< номер врат.
};

extern struct dlist_head crow_gateway_list;

__BEGIN_DECLS

void crow_gateway_init(struct crow_gateway *gate,
                       struct crow_gateway_operations *ops);

int crow_gateway_bind(struct crow_gateway *gate, int no);

void crow_gateway_deinit(struct crow_gateway *gate);

struct crow_gateway *crow_get_gateway(int no);

__END_DECLS

/**
    @brief Абстрактный класс врат. Врата отвечают за пересылку пакетов
    между башнями.
    @details Может некоторое время хранить отправляемые пакеты.
*/

namespace crow
{
    class gateway
    {
    public:
        struct dlist_head lnk = DLIST_HEAD_INIT(lnk); ///< встроенное поле списка.
        uint8_t id=0;            ///< номер врат.

        virtual void send(crow::packet *) = 0;
        virtual void finish() = 0;
        virtual void nblock_onestep() {}

        int bind(int gateno);

        gateway() = default;

        virtual ~gateway()
        {
            system_lock();
            dlist_del(&lnk);
            system_unlock();
        }
    };

    gateway *get_gateway(int no);

    extern struct dlist_head gateway_list;
}

#endif
