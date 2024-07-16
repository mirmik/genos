#include <crow/proto/node.h>
#include <igris/event/delegate.h>

namespace crow
{
    class node_delegate : public crow::node
    {
        igris::delegate<void, crow::node_packet_ptr> incom;
        igris::delegate<void, crow::node_packet_ptr> undeliv;

      public:
        node_delegate(igris::delegate<void, crow::node_packet_ptr> incom,
                      igris::delegate<void, crow::node_packet_ptr> undeliv)
            : incom(incom), undeliv(undeliv)
        {
        }

        void incoming_packet(crow_packet *pack) override
        {
            incom(crow::node_packet_ptr(pack));
        }

        void undelivered_packet(crow_packet *pack) override
        {
            undeliv(crow::node_packet_ptr(pack));
        }
    };
} // namespace crow
