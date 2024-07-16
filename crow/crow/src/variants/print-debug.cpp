#include <crow/print.h>
#include <crow/tower.h>
#include <igris/dprint.h>

void crow::diagnostic(const char *notation, crow::packet *pack)
{
    dpr(notation);
    dpr(": (");
    dpr("ptr:");
    dprptr(pack);
    dpr("qos:");
    dpr(pack->quality());
    dpr(",ack:");
    dpr((uint8_t)pack->ack());
    dpr(",atim:");
    dpr((uint16_t)pack->ackquant());
    dpr(",alen:");
    dpr((uint8_t)pack->addrsize());
    dpr(",flen:");
    dpr((uint8_t)pack->full_length());
    dpr(",type:");
    dpr((uint8_t)pack->type());
    dpr(",addr:");
    debug_writehex(pack->addrptr(), pack->addrsize());
    dpr(",stg:");
    dpr(pack->stage());
    dpr(",rescount:");
    dpr((uint8_t)pack->_ackcount);
    dpr(",data:");
    debug_write(pack->dataptr(), 20 > pack->datasize() ? pack->datasize() : 20);
    debug_putchar(')');
}
