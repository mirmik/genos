/**
 * @file
 * @author mirmik
 * @brief Управление памятью пакетов через libc malloc
 */

#include <crow/packet.h>
#include <stdlib.h>

#include <igris/dtrace.h>
#include <igris/sync/syslock.h>

int crow_allocated_count = 0;

void crow_deallocate_packet(crow_packet *pack)
{
    //	system_lock();
    if (pack)
        allocated_count--;
    free(pack);
    //	system_unlock();
}

crow_packet *crow_allocate_packet(size_t adlen)
{
    //	system_lock();
    allocated_count++;
    crow_packet *ret = (crow_packet *)malloc(adlen + sizeof(crow_packet));
    //	system_unlock();
    return ret;
}
