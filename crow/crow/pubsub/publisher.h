/** @file */

#ifndef CROW_PUBSUB_PUBLISHER_H
#define CROW_PUBSUB_PUBLISHER_H

#include "pubsub.h"

namespace crow
{
    class publisher
    {
    private:
        const char *theme;
        crow::hostaddr_view addr;
        uint8_t qos = 0;
        uint16_t acktime = 50;

    public:
        publisher() {}

        publisher(const crow::hostaddr_view &addr, const char *theme)
        {
            init(addr, theme);
        }

        void init(const crow::hostaddr_view &addr,
                  const char *theme,
                  uint8_t qos = 0,
                  uint16_t acktime = 50)
        {
            this->addr = addr;
            this->theme = theme;
            this->qos = qos;
            this->acktime = acktime;
        }

        void publish(const nos::buffer data)
        {
            crow::publish(addr, theme, data, qos, acktime);
        }

        void publish(const nos::buffer data, uint8_t qos, uint16_t acktime)
        {
            crow::publish(addr, theme, data, qos, acktime);
        }

        void publish_v(const nos::buffer *vec, int vecsz)
        {
            crow::publish_v(addr, theme, vec, vecsz, qos, acktime);
        }

        void publish_v(const nos::buffer *vec,
                       int vecsz,
                       uint8_t qos,
                       uint16_t acktime)
        {
            crow::publish_v(addr, theme, vec, vecsz, qos, acktime);
        }
    };
} // namespace crow
#endif