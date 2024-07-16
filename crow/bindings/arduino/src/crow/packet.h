/**
@file
@brief Всё, что касается работы с пакетом.
*/

#ifndef CROW_PACKAGE_H
#define CROW_PACKAGE_H

#include <stdint.h>
#include <stdlib.h>

#include <crow/defs.h>
#include <crow/hostaddr_view.h>

#include <igris/buffer.h>
#include <igris/container/pool.h>
#include <igris/datastruct/dlist.h>

/// Качество обслуживания.
#define CROW_WITHOUT_ACK 0
#define CROW_TARGET_ACK 1
#define CROW_BINARY_ACK 2

namespace crow
{
    struct gateway;

    /**
        @brief Структура заголовок пакета.
        @details Заголовок пакета располагается в первых байтах пакета.
        за заголовком следует поле адреса переменной длины, а за ним данные.
    */
    struct header
    {
        union
        {
            uint8_t pflag; ///< Флаги пакета
            struct
            {
                uint8_t ack : 1; ///< Идентифицирует ack пакеты. Доп.инф.
                                 ///< передается в типе.
                uint8_t RESERVED : 1;
                uint8_t noexec : 1; ///< Флаг предотвращает исполнение пакета.
                                    ///< Используется для запросов существования
                uint8_t type : 5; ///< Доп. инф. зависит от ситуации.
            } f;
        };
        uint16_t flen; ///< Полная длина пакета
        uint8_t alen;  ///< Длина поля адреса.
        uint8_t stg; ///< Поле стадии. Используется для того, чтобы цепочка врат
                     ///< знала, какую часть адреса обрабатывать.
        uint16_t ackquant; ///< Таймаут для пересылки пакета.
        uint16_t
            seqid; ///< Порядковый номер пакета. Присваивается отправителем.
        uint8_t qos; ///< Поле качества обслуживания.
    } __attribute__((packed));

    struct packet
    {
        struct dlist_head lnk =
            DLIST_HEAD_INIT(lnk); ///< Для подключения в списки башни crow.
        struct dlist_head ulnk =
            DLIST_HEAD_INIT(ulnk); ///< Для подключения в список пользователя и
                                   ///< зависимых протоколов.
        struct crow::gateway *ingate; ///< gate, которым пакет прибыл в систему.
        uint16_t last_request_time; ///< время последней отправки
        uint16_t _ackcount; ///< счетчик количества попыток отправки
        int8_t refs;
        union
        {
            uint8_t flags; ///< Местные флаги
            struct
            {
                uint8_t released_by_world : 1;
                uint8_t released_by_tower : 1;
                uint8_t confirmed : 1;
                uint8_t undelivered : 1;
                uint8_t sended_to_gate : 1;
            } f;
        };
        struct crow_header header;

        uint8_t *stageptr()
        {
            return (uint8_t *)(&header + 1) + header.stg;
        }

        char *endptr()
        {
            return (char *)(&header) + header.flen;
        }
        uint16_t blocksize()
        {
            return header.flen;
        }

        nos::buffer rawdata()
        {
            return nos::buffer(dataptr(), datasize());
        }
        crow::hostaddr_view addr()
        {
            return nos::buffer((char *)addrptr(), addrsize());
        }

        void revert_gate(uint8_t gateindex);
        void revert(nos::buffer *vec, size_t veclen);

        uint8_t *addrptr()
        {
            return (uint8_t *)(&header + 1);
        }
        uint8_t addrsize()
        {
            return header.alen;
        }

        char *dataptr()
        {
            return (char *)(addrptr() + addrsize());
        }
        uint16_t datasize()
        {
            return (uint16_t)(header.flen - header.alen - sizeof(crow_header));
        }

        size_t fullsize()
        {
            return header.flen;
        };

        void type(uint8_t t)
        {
            header.f.type = t;
        }
        void qos(uint8_t q)
        {
            header.qos = q;
        }
        void ackquant(uint16_t a)
        {
            header.ackquant = a;
        }

        uint8_t type()
        {
            return header.f.type;
        }
        uint8_t qos()
        {
            return header.qos;
        }
        uint16_t ackquant()
        {
            return header.ackquant;
        }

        void ackcount(uint16_t c)
        {
            _ackcount = c;
        }
        uint16_t ackcount()
        {
            return _ackcount;
        }
    }; //На самом деле, он не должен быть packed.
    //__attribute__((packed));

    /**
     * Выделить память для пакета.
     *
     * Выделяет adlen + sizeof(crow_packet) байт
     * @param adlen Суммарная длина адреса и данных в выделяемом пакете.
     */
    crow_packet *allocate_packet(size_t adlen);

    ///Вернуть память выделенную для пакета pack
    void deallocate_packet(crow_packet *pack);

    packet *create_packet(struct crow::gateway *ingate,
                          uint8_t addrsize,
                          size_t datasize);

    void packet_initialization(struct crow_packet *pack,
                               struct crow::gateway *ingate);

    // Только для аллокации через pool.
    void engage_packet_pool(void *zone, size_t zonesize, size_t elsize);
    igris::pool *get_package_pool();

    extern int allocated_count;
    bool has_allocated();

    void diagnostic(const char *label, crow_packet *pack);
}

#endif
