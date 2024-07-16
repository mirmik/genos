#ifndef NOS_INET_TCP_SERVER_H
#define NOS_INET_TCP_SERVER_H

#include <nos/inet/tcp_client.h>

namespace nos
{
    namespace inet
    {
        struct tcp_server : public inet::socket
        {
            tcp_server() = default;
            tcp_server(const tcp_server &oth) = default;
            tcp_server(tcp_server &&oth) = default;
            tcp_server(const nos::inet::hostaddr &addr,
                       uint16_t port,
                       int conn = 10);

            tcp_server &operator=(const tcp_server &oth) = default;
            tcp_server &operator=(tcp_server &&oth) = default;

            inet::tcp_client accept();

        public:
            int init();
            int bind(const nos::hostaddr &addr, uint16_t port);
            int listen();
            int listen(int conn);

            int close() override
            {
                return close_socket();
            }
        };
    }
}

#endif
