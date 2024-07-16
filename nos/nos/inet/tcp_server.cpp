#include <fcntl.h>
#include <nos/inet/tcp_client.h>
#include <nos/inet/tcp_server.h>

#ifdef __WIN32__
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(_MSC_VER)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif

nos::inet::tcp_server::tcp_server(const nos::inet::hostaddr &addr,
                                  uint16_t port,
                                  int conn)
{
    this->init();
    this->bind(addr, port);
    this->listen(conn);
}

int nos::inet::tcp_server::init()
{
    return socket::init(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

int nos::inet::tcp_server::bind(const nos::hostaddr &addr, uint16_t port)
{
    reusing(true);
    return socket::bind(addr, port, PF_INET);
}

int nos::inet::tcp_server::listen(int conn)
{
    return inet::socket::listen(conn);
}

int nos::inet::tcp_server::listen()
{
    auto sts = inet::socket::listen(10);
    return sts;
}

nos::inet::tcp_client nos::inet::tcp_server::accept()
{
    int c = sizeof(sockaddr_in);
    sockaddr_in caddr;
    memset(&caddr, 0, sizeof(caddr));
    int64_t cfd = ::accept((int)fd(), (sockaddr *)&caddr, (socklen_t *)&c);
    nos::inet::tcp_client sock;
    sock.init_from_socket(cfd, true);
    return sock;
}
