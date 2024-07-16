/**
@file packet.cpp
*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <crow/gateway.h>
#include <crow/packet.h>
#include <igris/sync/syslock.h>

#include <igris/compiler.h>

void crow_packet_initialization(crow_packet *pack, crow::gateway *ingate)
{
    dlist_init(&pack->lnk);
    dlist_init(&pack->ulnk);
    pack->ingate = ingate;
    pack->ackcount(5);
    pack->flags = 0;
    pack->refs = 0;
}

crow_packet *
crow_create_packet(crow::gateway *ingate, uint8_t addrsize, size_t datasize)
{
    crow_packet *pack = crow_allocate_packet(addrsize + datasize);

    if (pack == nullptr)
        return nullptr;

    pack->header.flen = (uint16_t)(sizeof(crow_header) + addrsize + datasize);
    pack->header.alen = addrsize;
    pack->header.ackquant = 200;
    pack->header.pflag = 0;
    pack->header.qos = 0;
    pack->header.stg = 0;

    packet_initialization(pack, ingate);

    return pack;
}

void crow_packet::revert_gate(uint8_t gateindex)
{
    *stageptr() = gateindex;
    ++header.stg;
}

void crow_packet::revert(nos::buffer *vec, size_t veclen)
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
    header.stg = (uint8_t)(header.stg + sz);
}

bool crow::has_allocated()
{
    return !!allocated_count;
}
