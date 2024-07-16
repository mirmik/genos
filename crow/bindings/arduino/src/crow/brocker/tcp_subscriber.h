/** @file */

#ifndef CROW_BROCKER_TCP_SUBSCRIBER_H
#define CROW_BROCKER_TCP_SUBSCRIBER_H

#include <map>
#include <nos/inet/tcp_socket.h>
#include <string>

#include "subscriber.h"

namespace crowker_implementation
{

    class tcp_subscriber : public subscriber
    {
      public:
        nos::inet::tcp_socket *sock;

        static std::map<nos::inet::netaddr, tcp_subscriber> allsubs;
        static tcp_subscriber *get(const nos::inet::netaddr addr)
        {
            return &allsubs[addr];
        }

        void publish(const std::string &theme, const std::string &data,
                     options *opts) override;
    };

} // namespace crowker_implementation

#endif