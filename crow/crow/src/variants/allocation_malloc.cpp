/**
 * @file
 * @author mirmik
 * @brief Управление памятью пакетов через libc malloc
 */

#include <crow/packet.h>
#include <igris/dtrace.h>
#include <igris/sync/syslock.h>
#include <stdlib.h>

int _crow_allocated_count = 0;

int crow_allocated_count_inc()
{
    system_lock();
    _crow_allocated_count++;
    system_unlock();
    assert(_crow_allocated_count < 64);
    return _crow_allocated_count;
}

int crow_allocated_count_dec()
{
    system_lock();
    _crow_allocated_count--;
    system_unlock();
    return _crow_allocated_count;
}

void crow::deallocate_packet(crow::packet *pack)
{
    if (pack)
    {
        crow_allocated_count_dec();
        free((void *)pack);
    }
    assert(_crow_allocated_count >= 0);
}

int crow::allocated_count()
{
    return _crow_allocated_count;
}

crow::packet *crow::allocate_packet_header_v0(int alen, int dlen)
{
    crow::packet *pack = allocate_packet_header_v0(alen + dlen);
    pack->set_addrsize(alen);
    pack->set_datasize(dlen);
    return pack;
}

crow::packet *crow::allocate_packet_header_v1(int alen, int dlen)
{
    crow::packet *pack = allocate_packet_header_v1(alen + dlen);
    pack->set_addrsize(alen);
    pack->set_datasize(dlen);
    return pack;
}

crow::packet *crow::allocate_packet_header_v0(int adlen)
{
    crow_allocated_count_inc();
    uint8_t *buffer = (uint8_t *)malloc(sizeof(crow::packet) +
                                        sizeof(crow::header_v0) + adlen);
    memset(buffer, 0, sizeof(crow::packet) + sizeof(crow::header_v0) + adlen);
    crow::packet *pack = new (buffer) crow::packet(crow::deallocate_packet);
    pack->attach_header((crow::header_v0 *)(buffer + sizeof(crow::packet)));
    pack->attach_addrdata(buffer + sizeof(crow::packet) +
                          sizeof(crow::header_v0));
    return pack;
}

crow::packet *crow::allocate_packet_header_v1(int adlen)
{
    crow_allocated_count_inc();
    uint8_t *buffer = (uint8_t *)malloc(sizeof(crow::packet) +
                                        sizeof(crow::header_v1) + adlen);
    memset(buffer, 0, sizeof(crow::packet) + sizeof(crow::header_v1) + adlen);
    crow::packet *pack = new (buffer) crow::packet(crow::deallocate_packet);
    pack->attach_header((crow::header_v1 *)(buffer + sizeof(crow::packet)));
    pack->attach_addrdata(buffer + sizeof(crow::packet) +
                          sizeof(crow::header_v1));
    return pack;
}
