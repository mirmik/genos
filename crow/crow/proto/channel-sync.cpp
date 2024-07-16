#include <crow/proto/channel.h>

int crow::channel::connect(const uint8_t *raddr, uint16_t rlen, nodeid_t rid,
                           uint8_t qos, uint16_t ackquant)
{
    handshake(raddr, rlen, rid, qos, ackquant);
    int ret = waitevent();

    return ret;
}
