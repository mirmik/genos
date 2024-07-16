/** @file */

#ifndef CROW_SENDER_H
#define CROW_SENDER_H

#include <crow/tower.h>

namespace crow
{
    class sender
    {
        uint8_t *addr;
        uint16_t alen;
        uint8_t type;
        uint8_t qos;
        uint16_t ackquant;

        sender(uint8_t *addr, uint16_t alen, uint8_t type, uint8_t qos,
               uint16_t ackquant)
            : addr(addr), alen(alen), type(type), qos(qos), ackquant(ackquant)
        {
        }

        packet_ptr send(const char *data, uint16_t dsize)
        {
            return crow::send(addr, alen, data, dsize, type, qos, ackquant);
        }
    };
}

#endif
