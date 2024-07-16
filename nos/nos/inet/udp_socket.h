#include <nos/inet/socket.h>
#include <tuple>

namespace nos
{
    namespace inet
    {
        /*class udp_socket : public socket
        {
        public:
            udp_socket();
            ~udp_socket();

            int
            sendto(const char *addr, int port, const void *data, size_t size);

            int sendto(const std::string &addr,
                       int port,
                       const void *data,
                       size_t size);

            int broadcast_send(const void *data, size_t size);

            int recvfrom(const char *&addr, int &port, void *data, size_t size);
            int recvfrom(std::string &addr, int &port, void *data, size_t size);
            int recvfrom(int &port, void *data, size_t size);
        };*/

        class udp_broadcast_socket : public socket
        {
        public:
            udp_broadcast_socket();
            udp_broadcast_socket(uint16_t port);
            ~udp_broadcast_socket();

            void allow_broadcast();
            void init();
            void bind(uint16_t port);

            int sendto(const void *data,
                        size_t size,
                        std::string ip,
                        uint16_t port);

            int send_broadcast(const void *data, size_t size, uint16_t port);
            int send_broadcast(const void *data, size_t size, uint16_t port, std::string broadcast_ip);

            std::tuple<std::string, std::string, uint16_t>
            recvfrom(size_t maxsize = 1 << 16);
        };
    }
}