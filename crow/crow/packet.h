/**
@file
@brief Всё, что касается работы с пакетом.
*/

#ifndef CROW_PACKAGE_H
#define CROW_PACKAGE_H

#include <crow/defs.h>
#include <crow/hostaddr_view.h>
#include <crow/qosbyte.h>
#include <igris/container/pool.h>
#include <igris/datastruct/dlist.h>
#include <igris/sync/syslock.h>
#include <nos/buffer.h>
#include <stdint.h>
#include <stdlib.h>

/// Качество обслуживания.
#define CROW_WITHOUT_ACK 0
#define CROW_TARGET_ACK 1
#define CROW_BINARY_ACK 2

namespace crow
{
    class gateway;
    class packet;
}

namespace crow
{
    /**
        @brief Структура заголовок пакета.
        @details Заголовок пакета располагается в первых байтах пакета.
        за заголовком следует поле адреса переменной длины, а за ним данные.
    */
    struct header_v1
    {
        union _u
        {
            uint8_t pflag = 0; ///< Флаги пакета
            struct _f
            {
                uint8_t ack : 1; ///< Идентифицирует ack пакеты. Доп.инф.
                ///< передается в типе.
                uint8_t RESERVED1 : 1;
                uint8_t RESERVED2 : 3;
                uint8_t type : 3; ///< Доп. инф. зависит от ситуации.
            } f;
        } u = {};
        uint16_t flen = 0; ///< Полная длина пакета
        uint8_t alen = 0;  ///< Длина поля адреса.
        uint8_t stg =
            0; ///< Поле стадии. Используется для того, чтобы цепочка врат
        ///< знала, какую часть адреса обрабатывать.
        uint16_t seqid = 0; ///< Порядковый номер пакета.
        qosbyte _qos = {}; ///< Поле качества обслуживания.

        void set_qos(uint8_t quality)
        {
            _qos.set_qos(quality);
        }

        void set_ackquant(uint16_t quant)
        {
            _qos.set_ackquant(quant);
        }

        uint8_t qos() const
        {
            return _qos.qos();
        }

        uint16_t ackquant() const
        {
            return _qos.ackquant();
        }

        uint16_t addrsize()
        {
            return alen;
        }

        uint16_t datasize()
        {
            return flen - alen - sizeof(header_v1);
        }

        void set_datasize(uint16_t sz)
        {
            flen = alen + sizeof(header_v1) + sz;
        }
    } __attribute__((packed));

    struct header_v0
    {
        union _u
        {
            uint8_t pflag = 0; ///< Флаги пакета
            struct _f
            {
                uint8_t ack : 1; ///< Идентифицирует ack пакеты. Доп.инф.
                ///< передается в типе.
                uint8_t RESERVED : 1;
                uint8_t noexec : 1; ///< Флаг предотвращает исполнение пакета.
                ///< Используется для запросов существования
                uint8_t type : 5; ///< Доп. инф. зависит от ситуации.
            } f;
        } u = {};
        uint16_t flen = 0; ///< Полная длина пакета
        uint8_t alen = 0;  ///< Длина поля адреса.
        uint8_t stg =
            0; ///< Поле стадии. Используется для того, чтобы цепочка врат
        ///< знала, какую часть адреса обрабатывать.
        uint16_t _ackquant = 0; ///< Таймаут для пересылки пакета.
        uint16_t seqid =
            0; ///< Порядковый номер пакета. Присваивается отправителем.
        uint8_t _qos = 0; ///< Поле качества обслуживания.

        void set_qos(uint8_t quality)
        {
            _qos = quality;
        }

        void set_ackquant(uint16_t quant)
        {
            _ackquant = quant;
        }

        uint8_t qos() const
        {
            return _qos;
        }

        uint16_t ackquant() const
        {
            return _ackquant;
        }

        uint16_t addrsize()
        {
            return alen;
        }

        uint16_t datasize()
        {
            return flen - alen - sizeof(header_v0);
        }

        void set_datasize(uint16_t size)
        {
            flen = size + alen + sizeof(header_v0);
        }

    } __attribute__((packed));

    class packet
    {
    public:
        packet(void (*destructor)(packet *)) : _destructor(destructor) {}

        packet(const packet &p) = delete;
        packet &operator=(const packet &p) = delete;

        struct dlist_head lnk =
            DLIST_HEAD_INIT(lnk); ///< Для подключения в списки башни crow.
        struct dlist_head ulnk =
            DLIST_HEAD_INIT(ulnk); ///< Для подключения в список пользователя и
        ///< зависимых протоколов.
        crow::gateway *ingate =
            nullptr; ///< gate, которым пакет прибыл в систему.
        uint16_t last_request_time = 0; ///< время последней отправки
        uint16_t _ackcount = 0; ///< счетчик количества попыток отправки
        int8_t refs = 0;
        union _u
        {
            uint8_t flags = 0; ///< Местные флаги
            struct _f
            {
                uint8_t released_by_world : 1;
                uint8_t released_by_tower : 1;
                uint8_t confirmed : 1;
                uint8_t undelivered : 1;
                uint8_t sended_to_gate : 1;
            } f;
        } u = {};

        union header_u
        {
            header_v1 *h1;
            header_v0 *h0;
        } h = {};
        uint8_t header_version = 1;
        uint8_t *_addrdata = nullptr; ///< Указатель на поле адреса

        // Эта функция вызывается для уничтожения пакета.
        // Она должна уничтожить сам пакет и все связанные с ним данные.
        void (*_destructor)(packet *) = nullptr;

    public:
        void attach_header(header_v0 *h)
        {
            this->h.h0 = h;
            header_version = 0;
        }

        void attach_header(header_v1 *h)
        {
            this->h.h1 = h;
            header_version = 1;
        }

        uint8_t *header_addr()
        {
            // h1 and h0 have the same value
            return (uint8_t *)h.h1;
        }

        void attach_addrdata(uint8_t *addrdata)
        {
            _addrdata = addrdata;
        }

        void revert_gate(uint8_t gateindex);
        void revert(nos::buffer *vec, size_t veclen);

        uint8_t *addrptr() const
        {
            return _addrdata;
        }

        uint8_t addrsize()
        {
            if (header_version == 1)
                return h.h1->addrsize();
            else
                return h.h0->addrsize();
        }

        char *dataptr()
        {
            return (char *)_addrdata + addrsize();
        }

        uint16_t datasize()
        {
            if (header_version == 1)
                return h.h1->datasize();
            else
                return h.h0->datasize();
        }

        char *endptr()
        {
            return dataptr() + datasize();
        }

        uint8_t *stageptr() const
        {
            return addrptr() + stage_index();
        }

        uint8_t stage_index() const
        {
            if (header_version == 1)
                return h.h1->stg;
            else
                return h.h0->stg;
        }

        uint16_t full_length() const
        {
            if (header_version == 1)
                return h.h1->flen;
            else
                return h.h0->flen;
        }

        uint8_t type() const
        {
            if (header_version == 1)
                return h.h1->u.f.type;
            else
                return h.h0->u.f.type;
        }

        uint8_t quality() const
        {
            if (header_version == 1)
                return h.h1->qos();
            else
                return h.h0->qos();
        }

        uint16_t ackquant() const
        {
            if (header_version == 1)
                return h.h1->ackquant();
            else
                return h.h0->ackquant();
        }

        uint8_t stage() const
        {
            return stage_index();
        }

        uint16_t seqid() const
        {
            if (header_version == 1)
                return h.h1->seqid;
            else
                return h.h0->seqid;
        }

        uint8_t ack() const
        {
            if (header_version == 1)
                return h.h1->u.f.ack;
            else
                return h.h0->u.f.ack;
        }

        void set_addrsize(uint8_t s)
        {
            if (header_version == 1)
                h.h1->alen = s;
            else
                h.h0->alen = s;
        }

        void set_datasize(uint16_t s)
        {
            if (header_version == 1)
                h.h1->set_datasize(s);
            else
                h.h0->set_datasize(s);
        }

        void set_type(uint8_t t)
        {
            if (header_version == 1)
                h.h1->u.f.type = t;
            else
                h.h0->u.f.type = t;
        }

        void set_quality(uint8_t q)
        {
            if (header_version == 1)
                h.h1->set_qos(q);
            else
                h.h0->set_qos(q);
        }

        void set_ackquant(uint16_t a)
        {
            if (header_version == 1)
                h.h1->set_ackquant(a);
            else
                h.h0->set_ackquant(a);
        }

        void set_stage(uint8_t s)
        {
            if (header_version == 1)
                h.h1->stg = s;
            else
                h.h0->stg = s;
        }

        void set_seqid(uint16_t s)
        {
            if (header_version == 1)
                h.h1->seqid = s;
            else
                h.h0->seqid = s;
        }

        void set_ack(uint8_t a)
        {
            if (header_version == 1)
                h.h1->u.f.ack = a;
            else
                h.h0->u.f.ack = a;
        }

        void increment_stage(int i)
        {
            set_stage(stage() + i);
        }

        void increment_seqid(int i)
        {
            set_seqid(seqid() + i);
        }

        void invalidate()
        {
            bool has_destructor = _destructor != nullptr;
            if (has_destructor)
                _destructor(this);
            else
                _destructor = nullptr;
        }

        size_t fullsize() const
        {
            return full_length();
        }

        ~packet()
        {
            invalidate();
        }

        void parse_header(const header_v1 &h)
        {
            set_ack(h.u.f.ack);
            set_type(h.u.f.type);
            set_quality(h.qos());
            set_ackquant(h.ackquant());
            set_seqid(h.seqid);
            set_stage(h.stg);
            set_addrsize(h.alen);
            set_datasize(h.flen - h.alen - sizeof(header_v1));
        }

        header_v1 extract_header_v1()
        {
            header_v1 h;
            h.seqid = seqid();
            h.u.f.ack = ack();
            h.u.f.type = type();
            h.set_qos(quality());
            h.set_ackquant(ackquant());
            h.stg = stage();
            h.flen = datasize() + addrsize() + sizeof(header_v1);
            h.alen = addrsize();
            h.stg = stage();
            return h;
        }

        header_v0 extract_header_v0()
        {
            header_v0 h;
            h.seqid = seqid();
            h.u.f.ack = ack();
            h.u.f.type = type();
            h.set_qos(quality());
            h.set_ackquant(ackquant());
            h.stg = stage();
            h.flen = datasize() + addrsize() + sizeof(header_v0);
            h.alen = addrsize();
            h.stg = stage();
            return h;
        }

        crow::hostaddr_view addr()
        {
            return crow::hostaddr_view(addrptr(), addrsize());
        }

        nos::buffer data()
        {
            return nos::buffer(dataptr(), datasize());
        }

        template <class T> T &subheader()
        {
            return *reinterpret_cast<T *>(dataptr());
        }
    };

    /**
     * Выделить память для пакета.
     *
     * Выделяет adlen + sizeof(crow::packet) байт
     * @param adlen Суммарная длина адреса и данных в выделяемом пакете.
     */
    crow::packet *allocate_packet_header_v1(int alen, int dlen);
    crow::packet *allocate_packet_header_v0(int alen, int dlen);
    crow::packet *allocate_packet_header_v1(int adlen);
    crow::packet *allocate_packet_header_v0(int adlen);
    void deallocate_compacted_packet(crow::packet *pack);
    void deallocate_packet(crow::packet *pack);
    void packet_initialization(crow::packet *pack, crow::gateway *ingate);
    crow::packet *
    create_packet(crow::gateway *ingate, uint8_t addrsize, size_t datasize);

    template <class Header> Header extract_header(crow::packet *pack)
    {
        static_assert(std::is_same<Header, crow::header_v1>::value ||
                      std::is_same<Header, crow::header_v0>::value);

        if constexpr (std::is_same<Header, crow::header_v1>::value)
        {
            return pack->extract_header_v1();
        }
        else if constexpr (std::is_same<Header, crow::header_v0>::value)
        {
            return pack->extract_header_v0();
        }
    }

    template <class Header> crow::packet *allocate_packet(int adlen)
    {
        static_assert(std::is_same<Header, crow::header_v1>::value ||
                      std::is_same<Header, crow::header_v0>::value);

        if constexpr (std::is_same<Header, crow::header_v1>::value)
        {
            return allocate_packet_header_v1(adlen);
        }
        else if constexpr (std::is_same<Header, crow::header_v0>::value)
        {
            return allocate_packet_header_v0(adlen);
        }
    }

    template <class Header> crow::packet *allocate_packet(int alen, int dlen)
    {
        static_assert(std::is_same<Header, crow::header_v1>::value ||
                      std::is_same<Header, crow::header_v0>::value);

        if constexpr (std::is_same<Header, crow::header_v1>::value)
        {
            return allocate_packet_header_v1(alen, dlen);
        }
        else if constexpr (std::is_same<Header, crow::header_v0>::value)
        {
            return allocate_packet_header_v0(alen, dlen);
        }
    }

    // Только для аллокации через pool.
    void engage_packet_pool(void *zone, size_t zonesize, size_t elsize);
    igris::pool *get_package_pool();

    bool has_allocated();

    void diagnostic(const char *label, crow::packet *pack);

    int allocated_count();
}

#endif
