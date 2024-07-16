/** @file */

#ifndef CROW_BROCKER_CROW_SUBSCRIBER_H
#define CROW_BROCKER_CROW_SUBSCRIBER_H

#include <map>
#include <string>

#include "options.h"
#include "subscriber.h"

namespace crowker_implementation
{
    class crow_subscriber : public subscriber
    {
      public:
        std::string addr;

        static std::map<std::string, crow_subscriber> allsubs;
        static crow_subscriber *get(const std::string &addr)
        {
            return &allsubs[addr];
        }

        void publish(const std::string &theme, const std::string &data,
                     options *opts) override;
    };
} // namespace crowker_implementation

#endif