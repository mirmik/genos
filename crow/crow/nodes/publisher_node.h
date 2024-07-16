#ifndef CROW_NODE_PUBLISHER_H
#define CROW_NODE_PUBLISHER_H

#include <crow/hostaddr.h>
#include <crow/proto/node.h>

namespace crow
{
    class abstract_publisher_node : public crow::node
    {
        bool _async = false;

    public:
        void set_async(bool en)
        {
            _async = en;
        }

        void publish_v(crow::hostaddr_view addr,
                       int crowker_node,
                       nos::buffer theme,
                       nos::buffer *data,
                       int len,
                       int qos,
                       int ackquant);

        void publish(crow::hostaddr_view addr,
                     int crowker_node,
                     nos::buffer theme,
                     nos::buffer data,
                     int qos,
                     int ackquant);
    };

    class publisher_node : public abstract_publisher_node
    {
    protected:
        crow::hostaddr crowker_addr = {};
        int crowker_node = CROWKER_SERVICE_BROCKER_NODE_NO;
        int qos = 0;
        int ackquant = 0;

        std::string theme = {};

    public:
        using abstract_publisher_node::publish;
        using abstract_publisher_node::publish_v;

        publisher_node() = default;

        publisher_node(crow::hostaddr_view crowker_addr,
                       int crowker_node,
                       nos::buffer theme);
        publisher_node(crow::hostaddr_view crowker_addr, nos::buffer theme);

        void init(crow::hostaddr_view crowker_addr, nos::buffer theme)
        {
            set_theme(theme);
            set_address(crowker_addr);
        }

        void init(crow::hostaddr_view crowker_addr,
                  nos::buffer theme,
                  int _qos,
                  int _ackquant)
        {
            set_theme(theme);
            set_address(crowker_addr);
            set_qos(_qos, _ackquant);
        }

        void publish(nos::buffer data);
        void publish_v(nos::buffer *data, int len);

        template <class T>
        void publish_timestamped_object(int64_t timestamp, const T &obj)
        {
            nos::buffer data[] = {{(void *)&timestamp, sizeof(timestamp)},
                                  {(void *)&obj, sizeof(obj)}};
            publish_v(data, 2);
        }

        void publish_timestamped_float(int64_t timestamp, float data)
        {
            publish_timestamped_object(timestamp, data);
        }

        void publish_timestamped_double(int64_t timestamp, double data)
        {
            publish_timestamped_object(timestamp, data);
        }

        void set_theme(nos::buffer theme);
        void set_address(crow::hostaddr_view addr);

        void set_qos(int _qos, int _ackquant)
        {
            qos = _qos;
            ackquant = _ackquant;
        }

    private:
        void incoming_packet(crow::packet *pack) override
        {
            crow::release(pack);
        }
    };

    // using publisher = publisher_node;
}

#endif
