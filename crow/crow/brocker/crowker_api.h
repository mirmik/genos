#ifndef CROW_CROWKER_API_H
#define CROW_CROWKER_API_H

#include <crow/brocker/client.h>
#include <crow/brocker/crowker.h>

namespace crow
{
    class crowker_api
    {
    public:
        virtual void subscribe_on_theme(crow::hostaddr_view,
                                        int nid,
                                        nos::buffer theme,
                                        uint8_t rqos,
                                        uint16_t rackquant) = 0;
        virtual void subscribe_on_theme_v2(crow::hostaddr_view,
                                           int nid,
                                           nos::buffer theme,
                                           uint8_t rqos,
                                           uint16_t rackquant,
                                           bool subscribe_to_updates,
                                           uint32_t request_latest) = 0;

        virtual void undelivered_packet_handle(crow::hostaddr_view,
                                               int node) = 0;
        virtual void
        client_beam(crow::hostaddr_view, int nid, nos::buffer name) = 0;
        virtual ~crowker_api() = default;

        virtual std::vector<crowker_implementation::client *> get_clients() = 0;

    protected:
        void publish_to_theme(nos::buffer theme,
                              const std::shared_ptr<std::string> &data)
        {
            crow::crowker::instance()->publish(theme.to_string(), data);
        }
    };
}

#endif