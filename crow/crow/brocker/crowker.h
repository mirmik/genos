/** @file */

#ifndef CROW_BROCKER_CROWKER_H
#define CROW_BROCKER_CROWKER_H

#include "crow_client.h"
#include "tcp_client.h"
#include "theme.h"
#include <crow/hostaddr.h>
#include <map>
#include <memory>
#include <nos/inet/tcp_client.h>
#include <string>

using namespace crowker_implementation;

int64_t crowker_eval_timestamp();

namespace crow
{
    class crowker_api;
    class crowker
    {
    public:
        std::map<std::string, crowker_implementation::theme> themes = {};
        std::vector<crowker_api *> apivec = {};
        bool brocker_info = false;
        bool log_publish = false;

    public:
        void set_info_mode(bool en)
        {
            brocker_info = en;
        }

        theme *get_theme(const std::string &name);
        void add_api(crowker_api *api)
        {
            apivec.push_back(api);
        }

        void publish(const std::string &theme,
                     const std::shared_ptr<std::string> &data);

        static crowker *instance()
        {
            static crowker *_instance = nullptr;
            if (_instance == nullptr)
                _instance = new crowker;

            return _instance;
        }

        void subscribe(const std::string &theme,
                       client *cl,
                       crowker_implementation::options opt);

        void send_latest(const std::string &theme,
                         client *cl,
                         uint32_t count_of_latest);

        void tcp_subscribe(const std::string &theme,
                           nos::inet::tcp_client *sock);

        void crow_subscribe(const crow::hostaddr_view &addr,
                            const std::string &theme,
                            uint8_t qos,
                            uint16_t ackquant);

        void unlink_theme_client(crowker_implementation::theme *thm,
                                 crowker_implementation::client *sub);

        void erase_crow_client(const std::string &addr)
        {
            crowker_implementation::crow_client::allsubs.erase(addr);
        }

        void erase_tcp_client(const nos::inet::netaddr &addr)
        {
            crowker_implementation::tcp_client::allsubs.erase(addr);
        }

        std::vector<client *> clients();

    private:
        crowker() = default;
    };
} // namespace crow

#endif