#ifndef NOS_INET_TCP_SOCKET_H
#define NOS_INET_TCP_SOCKET_H

#include <exception>
#include <nos/inet/hostaddr.h>
#include <nos/inet/socket.h>
#include <nos/io/iostream.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>

namespace nos
{
    namespace inet
    {
        class tcp_stream_error : public std::exception
        {
        public:
            const char *what() const noexcept override
            {
                return "tcp_stream_error";
            }
        };

        class tcp_write_error : public tcp_stream_error
        {
            const char *what() const noexcept override
            {
                return "tcp_write_error";
            }
        };

        class tcp_read_error : public tcp_stream_error
        {
            const char *what() const noexcept override
            {
                return "tcp_read_error";
            }
        };

        class tcp_connect_error : public tcp_stream_error
        {
            const char *what() const noexcept override
            {
                return "tcp_connect_error";
            }
        };

        class tcp_disconnect_error : public tcp_stream_error
        {
            const char *what() const noexcept override
            {
                return "tcp_disconnect_error";
            }
        };

        class tcp_timeout_error : public tcp_stream_error
        {
            const char *what() const noexcept override
            {
                return "tcp_timeout_error";
            }
        };

        class tcp_socket : public nos::inet::socket
        {
            // cannot be created explicit

        protected:
            tcp_socket() = default;
            tcp_socket(int fd) : nos::inet::socket(fd) {}

            //Копируется номер файлового дескриптора.
            tcp_socket(const tcp_socket &oth) = default;
            tcp_socket &operator=(const tcp_socket &oth) = default;

            //Перемещается номер файлового дескриптора.
            tcp_socket(tcp_socket &&oth) = default;
            tcp_socket &operator=(tcp_socket &&oth) = default;

            tcp_socket(nos::inet::hostaddr addr, uint16_t port);
            tcp_socket(nos::inet::netaddr naddr)
                : tcp_socket(naddr.addr, naddr.port)
            {
            }

            // create and connect
            int init();
            int connect(nos::inet::hostaddr addr, uint16_t port);

            netaddr getaddr();

            using nos::inet::socket::operator==;
            using nos::inet::socket::operator!=;
            using nos::inet::socket::operator>;
            using nos::inet::socket::operator<;
        };
    }
}

namespace std
{
    template <> struct hash<nos::inet::tcp_socket>
    {
        typedef nos::inet::tcp_socket argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const &s) const
        {
            return std::hash<int64_t>()(s.fd());
        }
    };
}

#endif