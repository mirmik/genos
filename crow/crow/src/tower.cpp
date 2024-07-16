/** @file */

#include <assert.h>
#include <limits>
#include <stdbool.h>
#include <string.h>

#include <igris/event/delegate.h>
#include <igris/sync/syslock.h>
#include <igris/time/systime.h>

#include <crow/gateway.h>
#include <crow/print.h>
#include <crow/proto/node.h>
#include <crow/pubsub/pubsub.h>
#include <crow/tower.h>
#include <crow/warn.h>

#include <nos/print.h>

#ifdef CROW_USE_ASYNCIO
#include <crow/asyncio.h>
#endif

bool crow::diagnostic_noack = false;
uint16_t crow::debug_data_size = 60;
unsigned int crow::total_travelled = 0;

bool crow::retransling_allowed = false;

DLIST_HEAD(crow_travelled);
DLIST_HEAD(crow_incoming);
DLIST_HEAD(crow_outters);

igris::delegate<void> crow::unsleep_handler;
void (*crow::default_incoming_handler)(crow::packet *pack) = nullptr;
void (*crow::undelivered_handler)(crow::packet *pack) = nullptr;

static bool __diagnostic_enabled = false;

bool _in_incoming_handler = false;
bool _in_undelivered_handler = false;

bool crow::diagnostic_enabled()
{
    return __diagnostic_enabled;
}
void crow::enable_diagnostic()
{
    __diagnostic_enabled = true;
}

void crow::diagnostic_setup(bool en)
{
    __diagnostic_enabled = en;
}

void crow::utilize(crow::packet *pack)
{
    dlist_del(&pack->lnk); // Очищается в tower_release((см. tower.c))
    dlist_del(&pack->ulnk);
    pack->invalidate();
}

void crow::release(crow::packet *pack)
{
    assert(pack);
    system_lock();

    if (pack->u.f.released_by_tower && pack->refs == 0)
        crow::utilize(pack);
    else
        pack->u.f.released_by_world = true;

    system_unlock();
}

void crow::tower_release(crow::packet *pack)
{
    system_lock();

    //Скорее всего это поле должно освобождаться без инициализации.
    //Инициализируем для последуещего освобождения в utilize
    dlist_del_init(&pack->lnk);

    if (pack->u.f.released_by_world && pack->refs == 0)
        crow::utilize(pack);
    else
        pack->u.f.released_by_tower = true;

    system_unlock();
}

static void confirmed_utilize_from_outers(crow::packet *pack)
{
    crow::packet *it;
    dlist_for_each_entry(it, &crow_outters, lnk)
    {
        if (it->seqid() == pack->seqid() &&
            pack->addrsize() == it->addrsize() &&
            !memcmp(it->addrptr(), pack->addrptr(), pack->addrsize()))
        {
            it->u.f.confirmed = 1;
            crow::node_protocol.delivered(it);
            crow::tower_release(it);
            return;
        }
    }
}

static void qos_release_from_incoming(crow::packet *pack)
{
    crow::packet *it;
    dlist_for_each_entry(it, &crow_incoming, lnk)
    {
        if (it->seqid() == pack->seqid() &&
            pack->addrsize() == it->addrsize() &&
            !memcmp(it->addrptr(), pack->addrptr(), pack->addrsize()))
        {
            crow::tower_release(it);
            return;
        }
    }
}

bool crow_time_comparator(crow::packet *a, crow::packet *b)
{
    uint64_t a_timer = a->last_request_time + a->ackquant();
    uint64_t b_timer = b->last_request_time + b->ackquant();

    return a_timer < b_timer;
}

static void add_to_incoming_list(crow::packet *pack)
{
    pack->last_request_time = igris::millis();
    dlist_move_sorted(pack, &crow_incoming, lnk, crow_time_comparator);
    crow::unsleep();
}

static void add_to_outters_list(crow::packet *pack)
{
    pack->last_request_time = igris::millis();
    dlist_move_sorted(pack, &crow_outters, lnk, crow_time_comparator);
    crow::unsleep();
}

crow::packet_ptr crow::travel(crow::packet *pack)
{
    system_lock();
    dlist_add_tail(&pack->lnk, &crow_travelled);
    system_unlock();
    crow::unsleep();
    return crow::packet_ptr(pack);
}

void crow::unsleep()
{
    if (unsleep_handler)
    {
        unsleep_handler();
    }

#ifdef CROW_USE_ASYNCIO
    crow::asyncio.unsleep();
#endif
}

static void crow_incoming_handler(crow::packet *pack)
{
    if (CROW_NODE_PROTOCOL == pack->type())
    {
        _in_incoming_handler = true;
        crow::node_protocol.incoming(pack);
        _in_incoming_handler = false;
        return;
    }

#ifdef CROW_PUBSUB_PROTOCOL_SUPPORTED
    if (CROW_PUBSUB_PROTOCOL == pack->type())
    {
        _in_incoming_handler = true;
        crow::pubsub_protocol.incoming(pack);
        _in_incoming_handler = false;
        return;
    }
#endif

    if (crow::default_incoming_handler)
    {
        _in_incoming_handler = true;
        crow::default_incoming_handler(pack);
        _in_incoming_handler = false;
        return;
    }
    else
    {
        if (__diagnostic_enabled)
        {
            crow::diagnostic("wproto", pack);
        }
        crow::release(pack);
        return;
    }
}

static void crow_send_ack(crow::packet *pack)
{
    crow::packet *ack = crow::create_packet(NULL, pack->addrsize(), 0);

    assert(pack);
    assert(ack);

    ack->set_type(pack->quality() == CROW_BINARY_ACK ? G1_ACK21_TYPE
                                                     : G1_ACK_TYPE);
    ack->set_ack(1);
    ack->set_quality(CROW_WITHOUT_ACK);
    ack->set_ackquant(pack->ackquant());
    ack->set_seqid(pack->seqid());
    memcpy(ack->addrptr(), pack->addrptr(), pack->addrsize());
    ack->u.f.released_by_world = true;
    crow::travel(ack);
}

static void crow_send_ack2(crow::packet *pack)
{
    crow::packet *ack = crow::create_packet(NULL, pack->addrsize(), 0);

    assert(pack);
    assert(ack);

    ack->set_type(G1_ACK22_TYPE);
    ack->set_ack(1);
    ack->set_quality(CROW_WITHOUT_ACK);
    ack->set_ackquant(pack->ackquant());
    ack->set_seqid(pack->seqid());
    memcpy(ack->addrptr(), pack->addrptr(), pack->addrsize());
    crow::travel(ack);
}

static void crow_revert_address(crow::packet *pack)
{
    uint8_t *first = pack->addrptr();
    uint8_t *last = pack->addrptr() + pack->addrsize();

    while ((first != last) && (first != --last))
    {
        char tmp = *last;
        *last = *first;
        *first++ = tmp;
    }
}

static void crow_tower_send_to_gate_phase(crow::packet *pack)
{
    uint8_t gidx = *pack->stageptr();
    crow::gateway *gate = crow::get_gateway(gidx);

    if (gate == NULL)
    {
        // second version compatible
        struct crow_gateway *gateway = crow_get_gateway(gidx);

        if (gateway)
        {
            if (pack->ack())
            {
                if (__diagnostic_enabled && crow::diagnostic_noack == false)
                    crow::diagnostic("track", pack);
            }
            else
            {
                if (__diagnostic_enabled)
                    crow::diagnostic("trans", pack);
            }
            //Здесь пакет штампуется временем отправки и пересылается во врата.
            //Врата должны после пересылки отправить его назад в башню
            //с помощью return_to_tower для контроля качества.
            assert(pack->u.f.sended_to_gate == 0);
            pack->u.f.sended_to_gate = 1;
            gateway->ops->send(gateway, pack);
        }

        if (__diagnostic_enabled)
            crow::diagnostic("wgate", pack);

        if (pack->ingate == nullptr)
        {
            crow::warn("wrong gate in out packet");
        }

        system_lock();
        crow::utilize(pack);
        system_unlock();
    }
    else
    {
        if (pack->ack())
        {
            if (__diagnostic_enabled && crow::diagnostic_noack == false)
                crow::diagnostic("track", pack);
        }
        else
        {
            if (__diagnostic_enabled)
                crow::diagnostic("trans", pack);
        }
        //Здесь пакет штампуется временем отправки и пересылается во врата.
        //Врата должны после пересылки отправить его назад в башню
        //с помощью return_to_tower для контроля качества.
        assert(pack->u.f.sended_to_gate == 0);
        pack->u.f.sended_to_gate = 1;
        gate->send(pack);
    }
}

static void crow_tower_incoming_ack_phase(crow::packet *pack)
{
    if (__diagnostic_enabled && crow::diagnostic_noack == false)
        crow::diagnostic("inack", pack);

    switch (pack->type())
    {
        case G1_ACK_TYPE:
            confirmed_utilize_from_outers(pack);
            break;

        case G1_ACK21_TYPE:
            confirmed_utilize_from_outers(pack);
            crow_send_ack2(pack);
            break;

        case G1_ACK22_TYPE:
            qos_release_from_incoming(pack);
            break;

        default:
            break;
    }

    system_lock();
    crow::utilize(pack);
    system_unlock();
}

static void crow_do_travel(crow::packet *pack)
{
    crow::total_travelled++;

    if (pack->stage() == pack->addrsize())
    {
        //Ветка доставленного пакета.
        crow_revert_address(pack);

        if (pack->ack())
        {
            //Перехватываем ack пакеты.
            crow_tower_incoming_ack_phase(pack);
            return;
        }

        if (__diagnostic_enabled)
            crow::diagnostic("incom", pack);

        if (pack->ingate)
        {
            //Если пакет пришел извне, используем логику обеспечения качества.

            //Для пакетов с подтверждение посылаем ack первого или второго
            //типов.
            if (pack->quality() == CROW_TARGET_ACK ||
                pack->quality() == CROW_BINARY_ACK)
                crow_send_ack(pack);

            if (pack->quality() == CROW_BINARY_ACK)
            {
                //Перед тем как добавить пакет в обработку, проверяем,
                //нет ли его в списке принятых.
                crow::packet *inc;
                dlist_for_each_entry(inc, &crow_incoming, lnk)
                {
                    if (inc->seqid() == pack->seqid() &&
                        inc->addrsize() == pack->addrsize() &&
                        memcmp(inc->addrptr(), pack->addrptr(),
                               inc->addrsize()) == 0)
                    {
                        // Пакет уже фигурирует как принятый, поэтому
                        // отбрасываем его.
                        system_lock();
                        crow::utilize(pack);
                        system_unlock();

                        return;
                    }
                }

                // Фиксирем пакет как принятый для фильтрации
                // возможных последующих копий.
                system_lock();
                add_to_incoming_list(pack);
                system_unlock();
            }
        }

        if (pack->ingate && pack->quality() != 2)
        {
            // Если пакет отправлен из данного нода, или не требуется
            // подтверждения второго уровня, обслуживание со стороны башни не
            // требуется. Вносим соответствующую пометку, что по crow_release
            // пакет мог быть удалён.
            crow::tower_release(pack);
        }

        //Решаем, что делать с пришедшим пакетом.
        crow_incoming_handler(pack);
        return;
    }
    else
    {

        //Ветка транзитного пакета. Логика поиска врат и пересылки.
        if (pack->ingate && crow::retransling_allowed == false)
        {
            static int warned = 0;
            if (warned == 0)
            {
                warned = 1;
#ifndef MEMORY_ECONOMY
                crow::warn(
                    "Crow get retransling request but retransling is not "
                    "allowed.\n"
                    "Set crow::retransling_allowed option for enable "
                    "retransling\n"
                    "Or use --retransler option if utility allowed it.\n");
#endif
            }

            system_lock();
            crow::utilize(pack);
            system_unlock();
            return;
        }

        crow_tower_send_to_gate_phase(pack);
    }
}

uint16_t __seqcounter = 0;
crow::packet_ptr crow_transport(crow::packet *pack, bool async)
{
    pack->set_stage(0);
    pack->set_ack(0);
    system_lock();
    pack->set_seqid(__seqcounter++);
    system_unlock();

    crow::packet_ptr ptr(pack);

    if (async)
    {
        return crow::travel(pack);
    }
    else
    {

        crow_do_travel(pack);

        // Делаем unsleep, чтобы перерасчитать таймауты.
        /*        if (crow::unsleep_handler && pack->quality() != 0)
                {
                    crow::unsleep_handler();
                }

         #ifdef CROW_USE_ASYNCIO
                crow::asyncio.unsleep();
        #endif
            }*/
        return ptr;
    }
}

void crow::nocontrol_travel(crow::packet *pack, bool fastsend)
{
    if (fastsend)
    {
        crow_do_travel(pack);
        return;
    }

    system_lock();
    dlist_add_tail(&pack->lnk, &crow_travelled);
    system_unlock();
}

crow::packet_ptr crow::send(const crow::hostaddr_view &addr,
                            const nos::buffer data,
                            uint8_t type,
                            uint8_t qos,
                            uint16_t ackquant,
                            bool async)
{
    crow::packet *pack = crow::create_packet(NULL, addr.size(), data.size());
    if (pack == nullptr)
    {
        crow::warn("cannot create packet");
        return nullptr;
    }

    pack->set_type(type & 0x1F);
    pack->set_quality(qos);
    pack->set_ackquant(ackquant);

    memcpy(pack->addrptr(), addr.data(), addr.size());
    memcpy(pack->dataptr(), data.data(), data.size());

    return crow_transport(pack, async);
}

crow::packet_ptr crow::send_v(const crow::hostaddr_view &addr,
                              const nos::buffer *vec,
                              size_t veclen,
                              uint8_t type,
                              uint8_t qos,
                              uint16_t ackquant,
                              bool async)
{
    size_t dsize = 0;
    const nos::buffer *it = vec;
    const nos::buffer *const eit = vec + veclen;

    for (; it != eit; ++it)
    {
        dsize += it->size();
    }

    crow::packet *pack = crow::create_packet(NULL, addr.size(), dsize);
    if (pack == nullptr)
        return nullptr;

    pack->set_type(type & 0x1F);
    pack->set_quality(qos);
    pack->set_ackquant(ackquant);

    memcpy(pack->addrptr(), addr.data(), addr.size());

    it = vec;
    char *dst = pack->dataptr();

    for (; it != eit; ++it)
    {
        memcpy(dst, it->data(), it->size());
        dst += it->size();
    }

    return crow_transport(pack, async);
}

crow::packet_ptr crow::send_vv(const crow::hostaddr_view &addr,
                               const nos::buffer *vec,
                               size_t veclen,
                               const nos::buffer *vec2,
                               size_t veclen2,
                               uint8_t type,
                               uint8_t qos,
                               uint16_t ackquant,
                               bool async)
{
    size_t dsize = 0;
    const nos::buffer *it;
    const nos::buffer *eit;

    it = vec;
    eit = vec + veclen;
    for (; it != eit; ++it)
    {
        dsize += it->size();
    }

    it = vec2;
    eit = vec2 + veclen2;
    for (; it != eit; ++it)
    {
        dsize += it->size();
    }

    crow::packet *pack = crow::create_packet(NULL, addr.size(), dsize);
    if (pack == nullptr)
        return nullptr;

    pack->set_type(type & 0x1F);
    pack->set_quality(qos);
    pack->set_ackquant(ackquant);

    memcpy(pack->addrptr(), addr.data(), addr.size());
    char *dst = pack->dataptr();

    it = vec;
    eit = vec + veclen;
    for (; it != eit; ++it)
    {
        memcpy(dst, it->data(), it->size());
        dst += it->size();
    }

    it = vec2;
    eit = vec2 + veclen2;
    for (; it != eit; ++it)
    {
        memcpy(dst, it->data(), it->size());
        dst += it->size();
    }

    return crow_transport(pack, async);
}

crow::packet_ptr crow::send_vvv(const crow::hostaddr_view &addr,
                                const nos::buffer *vec,
                                size_t veclen,
                                const nos::buffer *vec2,
                                size_t veclen2,
                                const nos::buffer *vec3,
                                size_t veclen3,
                                uint8_t type,
                                uint8_t qos,
                                uint16_t ackquant,
                                bool async)
{
    size_t dsize = 0;
    const nos::buffer *it;
    const nos::buffer *eit;

    it = vec;
    eit = vec + veclen;
    for (; it != eit; ++it)
    {
        dsize += it->size();
    }

    it = vec2;
    eit = vec2 + veclen2;
    for (; it != eit; ++it)
    {
        dsize += it->size();
    }

    it = vec3;
    eit = vec3 + veclen3;
    for (; it != eit; ++it)
    {
        dsize += it->size();
    }

    crow::packet *pack = crow::create_packet(NULL, addr.size(), dsize);
    if (pack == nullptr)
        return nullptr;

    pack->set_type(type & 0x1F);
    pack->set_quality(qos);
    pack->set_ackquant(ackquant);

    memcpy(pack->addrptr(), addr.data(), addr.size());
    char *dst = pack->dataptr();

    it = vec;
    eit = vec + veclen;
    for (; it != eit; ++it)
    {
        memcpy(dst, it->data(), it->size());
        dst += it->size();
    }

    it = vec2;
    eit = vec2 + veclen2;
    for (; it != eit; ++it)
    {
        memcpy(dst, it->data(), it->size());
        dst += it->size();
    }

    it = vec3;
    eit = vec3 + veclen3;
    for (; it != eit; ++it)
    {
        memcpy(dst, it->data(), it->size());
        dst += it->size();
    }

    return crow_transport(pack, async);
}

void crow::return_to_tower(crow::packet *pack, uint8_t sts)
{
    pack->u.f.sended_to_gate = 0;
    assert(pack);

    system_lock();

    if (pack->ingate != NULL)
    {
        //Пакет был отправлен, и он нездешний. Уничтожить.
        crow::utilize(pack);
    }
    else
    {
        //Пакет здешний.
        if (sts != CROW_SENDED || pack->quality() == CROW_WITHOUT_ACK)
            crow::tower_release(pack);
        else
            add_to_outters_list(pack);
    }

    system_unlock();
}

void crow_undelivered(crow::packet *pack)
{
    pack->u.f.undelivered = 1;

    if (crow::undelivered_handler)
    {
        _in_undelivered_handler = true;
        crow::undelivered_handler(pack);
        _in_undelivered_handler = false;
    }

    if (CROW_NODE_PROTOCOL == pack->type())
    {
        _in_undelivered_handler = true;
        crow::node_protocol.undelivered(pack);
        _in_undelivered_handler = false;
        return;
    }

#ifdef CROW_PUBSUB_PROTOCOL_SUPPORTED
    else if (CROW_PUBSUB_PROTOCOL == pack->type())
    {
        _in_undelivered_handler = true;
        crow::pubsub_protocol.undelivered(pack);
        _in_undelivered_handler = false;
        return;
    }
#endif
}

static inline void crow_onestep_send_stage()
{
    crow::packet *pack;

    system_lock();

    while (1)
    {

        if (dlist_empty(&crow_travelled))
        {
            break;
        }

        pack = dlist_first_entry(&crow_travelled, crow::packet, lnk);
        dlist_del_init(&pack->lnk);

        system_unlock();
        crow_do_travel(pack);

        system_lock();
    }
    system_unlock();
}

void crow_onestep_outers_stage()
{
    crow::packet *pack;
    crow::packet *n;

    uint16_t curtime = igris::millis();

    system_lock();
    if (dlist_empty(&crow_outters))
    {
        system_unlock();
        return;
    }

    dlist_for_each_entry_safe(pack, n, &crow_outters, lnk)
    {
        assert(pack->u.f.released_by_tower == 0);

        if (curtime - pack->last_request_time >= pack->ackquant())
        {
            dlist_del_init(&pack->lnk);

            if (pack->_ackcount != 0xFFFF)
                --pack->_ackcount;

            if (pack->_ackcount == 0)
            {
                crow_undelivered(pack);
                crow::tower_release(pack);
            }
            else
            {
                system_unlock();
                crow::travel(pack);
                system_lock();
            }
        }
    }
    system_unlock();
}

void crow_onestep_incoming_stage()
{
    crow::packet *pack;
    crow::packet *n;

    uint16_t curtime = igris::millis();

    system_lock();
    if (dlist_empty(&crow_incoming))
    {
        system_unlock();
        return;
    }

    dlist_for_each_entry_safe(pack, n, &crow_incoming, lnk)
    {
        assert(pack->u.f.released_by_tower == 0);

        if (curtime - pack->last_request_time >= pack->ackquant())
        {
            dlist_del_init(&pack->lnk);

            if (pack->_ackcount != 0xFFFF)
                --pack->_ackcount;

            if (pack->_ackcount == 0)
            {
                pack->u.f.undelivered = 1;
                crow::utilize(pack);
            }
            else
            {
                dlist_move_sorted(pack, &crow_incoming, lnk,
                                  crow_time_comparator);
                pack->last_request_time = curtime;
                system_unlock();
                crow_send_ack(pack);
                system_lock();
            }
        }
    }
    system_unlock();
}

void crow_onestep_keepalive_stage()
{
    crow::keepalive_timer_manager.exec(igris::millis());
}

void crow::onestep()
{
#ifndef CROW_USE_ASYNCIO
    crow::gateway *gate;
    dlist_for_each_entry(gate, &crow::gateway_list, lnk)
    {
        gate->nblock_onestep();
    }
#endif

    crow_onestep_send_stage();
    crow_onestep_keepalive_stage();
    crow_onestep_outers_stage();
    crow_onestep_incoming_stage();
}

int crow::incomming_stage_count()
{
    return dlist_size(&crow_incoming);
}
int crow::outers_stage_count()
{
    return dlist_size(&crow_outters);
}

bool crow::has_untravelled()
{
    system_lock();

    int ret = !(dlist_empty(&crow_travelled) && dlist_empty(&crow_outters) &&
                dlist_empty(&crow_incoming));

    system_unlock();
    return ret;
}

bool crow::has_untravelled_now()
{
    system_lock();

    int ret = !dlist_empty(&crow_travelled);

    system_unlock();
    return ret;
}

void crow::finish()
{
    crow::gateway *gate;
    dlist_for_each_entry(gate, &crow::gateway_list, lnk)
    {
        gate->finish();
    }
}

bool crow::fully_empty()
{
    system_lock();
    int ret = dlist_empty(&crow_travelled) && dlist_empty(&crow_incoming) &&
              dlist_empty(&crow_outters);
    system_unlock();
    return ret;
}

int64_t crow::get_minimal_timeout()
{
    // TODO : Ошибки в учёте переходов через uint16_t

    // int64_t result;
    int64_t mininterval = std::numeric_limits<int64_t>::max();
    int64_t curtime = igris::millis();

    auto update_candidate = [&](int64_t candidate) {
        if (mininterval > candidate)
            mininterval = candidate;
    };

    if (!keepalive_timer_manager.empty())
    {
        update_candidate(keepalive_timer_manager.minimal_interval(curtime));
    }

    if (!dlist_empty(&crow_incoming))
    {
        crow::packet *i = dlist_first_entry(&crow_incoming, crow::packet, lnk);
        update_candidate(i->last_request_time + i->ackquant() - curtime);
    }

    if (!dlist_empty(&crow_outters))
    {
        crow::packet *o = dlist_first_entry(&crow_outters, crow::packet, lnk);
        update_candidate(o->last_request_time + o->ackquant() - curtime);
    }

    if (mininterval == std::numeric_limits<int64_t>::max())
        return -1;

    else if (mininterval < 0)
        return 0;

    else
        return mininterval;
}
