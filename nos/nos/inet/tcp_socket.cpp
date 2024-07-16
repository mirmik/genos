#include <nos/inet/tcp_socket.h>

#ifdef __WIN32__
#include <winsock2.h>
#include <ws2tcpip.h>
#elif defined(_MSC_VER)
#include <fcntl.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#endif

nos::inet::tcp_socket::tcp_socket(nos::inet::hostaddr addr, uint16_t port)
    : tcp_socket()
{
    inet::socket::init(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(addr, port);
}

int nos::inet::tcp_socket::init()
{
    return inet::socket::init(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

int nos::inet::tcp_socket::connect(nos::inet::hostaddr addr, uint16_t port)
{
    auto sts = socket::connect(addr, port, PF_INET);
    reusing(true);
    return sts;
}

nos::expected<size_t, nos::output_error>
nos::inet::socket::send(const void *data, size_t size, int flags)
{
    return (size_t)::send(fd(), (const char *)data, (int)size, flags);
}

nos::expected<size_t, nos::input_error>
nos::inet::socket::recv(char *data, size_t size, int flags)
{
    return (size_t)::recv(fd(), data, (int)size, flags);
}

nos::inet::netaddr nos::inet::tcp_socket::getaddr()
{
    struct sockaddr_in sin;
    socklen_t socklen = sizeof(sin);
    memset(&sin, 0, sizeof(sin));

    getpeername(fd(), (struct sockaddr *)&sin, &socklen); // read binding

    return nos::inet::netaddr{sin.sin_addr.s_addr, sin.sin_port};
}

/*int nos::inet::socket::clean()
{
#ifndef __WIN32__
    char buf[16];
    nos::expected<size_t, nos::input_error> ret;
    do
    {
        nos::osutil::nonblock(fd(), true);
        ret = this->recv(buf, 16, 0);
        if (ret.is_error())
        {
            return 0;
        }
    } while (*ret > 0);
#endif
    return 0;
}*/
