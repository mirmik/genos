#include <crow/packet.h>
#include <igris/container/pool.h>

#include <igris/sync/syslock.h>

extern bool __live_diagnostic_enabled;
igris::pool _crow_packet_pool;
int crow_allocated_count = 0;

igris::pool *crow::get_package_pool() { return &_crow_packet_pool; }

void crow::engage_packet_pool(void *zone, size_t zonesize, size_t elsize)
{
    _crow_packet_pool.init(zone, zonesize, elsize);
}

void crow::deallocate_packet(crow::packet *pack)
{
    assert(pack);

    system_lock();

    if (pack)
        crow_allocated_count--;

    _crow_packet_pool.put(pack);
    system_unlock();
}

crow::compacted_packet *crow_allocate_packet(size_t adlen)
{
    (void) adlen;
    system_lock();
    void *ret = _crow_packet_pool.get();
    memset(ret, 0, _crow_packet_pool.element_size());
    new (ret) crow::compacted_packet;

    if (ret)
        crow_allocated_count++;

    system_unlock();
    pack->set_destructor(crow::deallocate_packet);

    return (crow::compacted_packet *)ret;
}
