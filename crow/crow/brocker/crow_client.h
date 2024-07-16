/** @file */

#ifndef CROW_BROCKER_CROW_SUBSCRIBER_H
#define CROW_BROCKER_CROW_SUBSCRIBER_H

#include <map>
#include <string>

#include "client.h"
#include "options.h"

namespace crowker_implementation
{
    class crow_client : public client
    {
    public:
        std::string addr = {};

        static std::map<std::string, crow_client> allsubs;
        static crow_client *get(const std::string &addr)
        {
            return &allsubs[addr];
        }

        static std::vector<client *> clients()
        {
            std::vector<client *> ret;
            for (auto &pair : allsubs)
                ret.push_back(&pair.second);
            return ret;
        }

        void publish(const std::string &theme,
                     const std::string &data,
                     options opts) override;
    };
} // namespace crowker_implementation

#endif