#include <crow/proto/node.h>

namespace crow
{
    class pynode_delegate : public crow::node
    {
        std::function<void(crow::node_packet_ptr)> incom;
        std::function<void(crow::node_packet_ptr)> undeliv;

    public:
        pynode_delegate(std::function<void(crow::node_packet_ptr)> incom,
                        std::function<void(crow::node_packet_ptr)> undeliv)
            : incom(incom), undeliv(undeliv)
        {
        }

        void incoming_packet(crow::packet *pack) override
        {
            incom(crow::node_packet_ptr(pack, this));
        }

        void undelivered_packet(crow::packet *pack) override
        {
            undeliv(crow::node_packet_ptr(pack, this));
        }
    };
}
