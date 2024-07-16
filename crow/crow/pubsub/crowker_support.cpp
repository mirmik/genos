/** @file */

#include "pubsub.h"
#include "subscriber.h"
#include <crow/brocker/crowker.h>

#include <igris/util/bug.h>

void incoming_crowker_handler(crow::packet *pack)
{
    crow::subheader_pubsub &shps = pack->subheader<crow::subheader_pubsub>();

    switch (shps.type)
    {
        case (uint8_t)crow::pubsub_type::PUBLISH:
        {
            auto &shps_d = pack->subheader<crow::subheader_pubsub_data>();
            auto theme = shps_d.theme().to_string();
            auto data = std::make_shared<std::string>(shps_d.data().data(),
                                                      shps_d.data().size());
            crow::crowker::instance()->publish(theme, data);
        }
        break;

        case (uint8_t)crow::pubsub_type::SUBSCRIBE:
        {
            auto &shps_c = pack->subheader<crow::subheader_pubsub_control>();
            std::string theme = shps_c.theme().to_string();

            crow::crowker::instance()->crow_subscribe(
                {pack->addrptr(), pack->addrsize()}, theme, shps_c.qos,
                shps_c.ackquant);
        }
        break;

        case (uint8_t)crow::pubsub_type::SERVICE_ANSWER:
        {
            BUG();
        }
        break;

        default:
        {
            printf("unresolved pubsub frame type %d", (uint8_t)shps.type);
        }
        break;
    }

    crow::release(pack);
}

void undelivered_crowker_handler(crow::packet *pack)
{
    auto &shps = pack->subheader<crow::subheader_pubsub>();

    if (shps.type == (uint8_t)crow::pubsub_type::PUBLISH)
    {
        crow::crowker::instance()->erase_crow_client(
            std::string((char *)pack->addrptr(), pack->addrsize()));
    }

    crow::release(pack);
}

void crow::pubsub_protocol_cls::enable_crowker_subsystem()
{
    instance().incoming_handler = incoming_crowker_handler;
    instance().undelivered_handler = undelivered_crowker_handler;
}
