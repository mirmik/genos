/** @file */

#ifndef CROW_GATEWAY_H
#define CROW_GATEWAY_H

#include <crow/packet.h>
#include <igris/datastruct/dlist.h>
#include <igris/sync/syslock.h>

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
        struct dlist_head lnk; ///< встроенное поле списка.
        uint8_t id;            ///< номер врат.

        virtual void send(crow_packet *) = 0;
        virtual void nblock_onestep() = 0;
        virtual void finish() = 0;

        int bind(int gateno);
#if !CROW_ENABLE_WITHOUT_FDS
        virtual int get_fd() { return -1; }
#endif

        gateway() { dlist_init(&lnk); }

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
