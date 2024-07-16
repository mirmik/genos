/** @file */

#ifndef CROW_BROCKER_TCP_SUBSCRIBER_H
#define CROW_BROCKER_TCP_SUBSCRIBER_H

#include <map>
#include <nos/inet/tcp_socket.h>
#include <string>

#include "client.h"

namespace crowker_implementation
{

    class tcp_client : public client
    {
    public:
        static std::map<nos::inet::netaddr, tcp_client> allsubs;

    public:
        nos::inet::tcp_socket *sock = nullptr;

        static tcp_client *get(const nos::inet::netaddr addr)
        {
            return &allsubs[addr];
        }

        void publish(const std::string &theme,
                     const std::string &data,
                     options opts) override;

        static std::vector<client *> clients()
        {
            std::vector<client *> ret;
            for (auto &pair : allsubs)
                ret.push_back(&pair.second);
            return ret;
        }
    };
}

#endif