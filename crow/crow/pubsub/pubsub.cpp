/** @file */

#include "pubsub.h"
#include "subscriber.h"
#include <utility>

#include <crow/hexer.h>
#include <igris/sync/syslock.h>
#include <igris/util/size.h>

crow::pubsub_protocol_cls crow::pubsub_protocol;

crow::pubsub_protocol_cls &crow::pubsub_protocol_cls::instance()
{
    return crow::pubsub_protocol;
}

void crow::pubsub_protocol_cls::incoming(crow::packet *pack)
{
    if (incoming_handler)
    {
        incoming_handler(pack);
    }
    else
    {
        auto &shdr = pack->subheader<crow::subheader_pubsub_data>();
        nos::buffer theme = shdr.theme();

        crow::subscriber *sub;
        dlist_for_each_entry(sub, &subscribers, lnk)
        {
            if (theme == sub->theme)
            {
                sub->newpack_handler(pack);
                return;
            }
        }
        crow::release(pack);
    }
}

void crow::pubsub_protocol_cls::undelivered(crow::packet *pack)
{
    if (undelivered_handler)
    {
        undelivered_handler(pack);
    }
    else
    {
        crow::release(pack);
    }
}

crow::packet_ptr crow::publish(const crow::hostaddr_view &addr,
                               const nos::buffer theme,
                               const nos::buffer data,
                               uint8_t qos,
                               uint16_t acktime,
                               uint8_t type)
{
    crow::subheader_pubsub_data subps_d;

    subps_d.type = type;
    subps_d.thmsz = theme.size();
    subps_d.datsz = data.size();

    const nos::buffer iov[] = {
        {(char *)&subps_d, sizeof(subps_d)},
        {(char *)theme.data(), subps_d.thmsz},
        data,
    };

    return crow::send_v(addr, iov, igris::size(iov), CROW_PUBSUB_PROTOCOL, qos,
                        acktime);
}

crow::packet_ptr crow::publish_v(const crow::hostaddr_view &addr,
                                 const nos::buffer theme,
                                 const nos::buffer *vec,
                                 int vecsz,
                                 uint8_t qos,
                                 uint16_t acktime)
{
    crow::subheader_pubsub_data subps_d;

    subps_d.type = (uint8_t)crow::pubsub_type::PUBLISH;
    subps_d.thmsz = theme.size();
    subps_d.datsz = 0;

    for (int i = 0; i < vecsz; i++)
    {
        subps_d.datsz += vec[i].size();
    }

    const nos::buffer iov[] = {
        {(char *)&subps_d, sizeof(subps_d)},
        {(char *)theme.data(), subps_d.thmsz},
    };

    return crow::send_vv(addr, iov, igris::size(iov), vec, vecsz,
                         CROW_PUBSUB_PROTOCOL, qos, acktime);
}

void crow::subscribe(const crow::hostaddr_view &addr,
                     const nos::buffer theme,
                     uint8_t qos,
                     uint16_t acktime,
                     uint8_t rqos,
                     uint16_t racktime)
{
    size_t thmsz = theme.size();
    crow::subheader_pubsub_control subps_c;

    subps_c.type = (uint8_t)crow::pubsub_type::SUBSCRIBE;
    subps_c.thmsz = (uint8_t)thmsz;
    subps_c.qos = rqos;
    subps_c.ackquant = racktime;

    const nos::buffer iov[] = {
        {(char *)&subps_c, sizeof(subps_c)},
        {(char *)theme.data(), theme.size()},
    };

    crow::send_v(addr, iov, igris::size(iov), CROW_PUBSUB_PROTOCOL, qos,
                 acktime);
}

void crow::pubsub_protocol_cls::resubscribe_all()
{
    crow::subscriber *sub;

    system_lock();
    dlist_for_each_entry(sub, &subscribers, lnk)
    {
        system_unlock();
        sub->resubscribe();
        system_lock();
    }
    system_unlock();
}
