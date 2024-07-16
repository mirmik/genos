/**
@file packet.cpp
*/

#include <crow/gateway.h>
#include <crow/packet.h>
#include <igris/compiler.h>
#include <igris/sync/syslock.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int crow_allocated_count = 0;

void crow::packet_initialization(crow::packet *pack, crow::gateway *ingate)
{
    dlist_init(&pack->lnk);
    dlist_init(&pack->ulnk);
    pack->ingate = ingate;
    pack->_ackcount = 5;
    pack->u.flags = 0;
    pack->refs = 0;
}

crow::packet *
crow::create_packet(crow::gateway *ingate, uint8_t addrsize, size_t datasize)
{
    crow::packet *pack =
        crow::allocate_packet<crow::header_v1>(addrsize, datasize);
    if (pack == nullptr)
        return nullptr;
    pack->set_ackquant(200);
    pack->set_quality(0);
    pack->set_stage(0);
    crow::packet_initialization(pack, ingate);
    return pack;
}

// Номер врат - 1 байт, так что просто меняем одно число на
// номер врат, через которые проходит пакет.
void crow::packet::revert_gate(uint8_t gateindex)
{
    *stageptr() = gateindex;
    increment_stage(1);
}

// Перезаписываем информацию на ту информацию, какую предоставили врата
// в качестве обратного адреса.
void crow::packet::revert(nos::buffer *vec, size_t veclen)
{
    nos::buffer *it = vec + veclen - 1;
    nos::buffer *eit = vec - 1;

    size_t sz = 0;
    uint8_t *tgt = stageptr();

    for (; it != eit; --it)
    {
        sz += it->size();
        char *ptr = (char *)it->data() + it->size();
        char *eptr = (char *)it->data();
        while (ptr != eptr)
            *tgt++ = *--ptr;
    }
    increment_stage(sz);
}

bool crow::has_allocated()
{
    return !!crow_allocated_count;
}
