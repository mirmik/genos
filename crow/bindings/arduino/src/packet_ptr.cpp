#include <crow/packet_ptr.h>
#include <crow/tower.h>

#include <assert.h>

crow::packet_ptr::~packet_ptr() { clear(); }

void crow::packet_ptr::clear()
{
    if (pack)
    {
        pack->refs--;
        assert(pack->refs >= 0);

        if (pack->refs == 0)
            crow::release(pack);

        pack = nullptr;
    }
}
