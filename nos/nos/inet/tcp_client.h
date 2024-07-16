#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <chrono>
#include <nos/inet/tcp_socket.h>
#include <string_view>

using namespace std::chrono_literals;

namespace nos
{
    namespace inet
    {
        class tcp_client : public nos::inet::tcp_socket
        {
            bool _is_connect = false;
            nos::inet::hostaddr _addr = {};
            uint16_t _port = 0;

        public:
            tcp_client() = default;
            tcp_client(int fd) : tcp_socket(fd) {}
            tcp_client(const tcp_client &oth) = default;
            tcp_client &operator=(const tcp_client &oth) = default;

            tcp_client(nos::inet::netaddr naddr)
                : tcp_client(naddr.addr, naddr.port)
            {
            }
            tcp_client(nos::inet::hostaddr addr, uint16_t port);

            std::string ip()
            {
                return _addr.to_string();
            }

            uint16_t port()
            {
                return _port;
            }

            void init_from_socket(int64_t fd, bool connected)
            {
                set_fd(fd);
                this->_is_connect = connected;
            }

            bool connected() const
            {
                return _is_connect;
            }

            bool is_connected()
            {
                return _is_connect;
            }

            nos::expected<size_t, nos::output_error>
            write(const void *data, size_t size) override;
            nos::expected<size_t, nos::input_error> read(void *data,
                                                         size_t size) override;

            int connect(nos::inet::hostaddr addr,
                        uint16_t port,
                        std::chrono::milliseconds timeout);
            int connect(nos::inet::hostaddr addr, uint16_t port);
            int disconnect();
            using istream::read;

            int send(const void *data, size_t size)
            {
                return *write(data, size);
            }

            int send(const std::string_view &data)
            {
                return send(data.data(), data.size());
            }

            static tcp_client dial(nos::inet::hostaddr addr,
                                   uint16_t port,
                                   std::chrono::milliseconds timeout = 3250ms);

            netaddr getaddr()
            {
                return tcp_socket::getaddr();
            }
        };
    }
}

#endif // TCP_CLIENT_H