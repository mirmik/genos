#ifndef GENOS_SOCKET_H
#define GENOS_SOCKET_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/tcp.h>

#include "string.h"

class LinuxSocket
{
public:
	int sockfd;

	void init(int fd)
	{
		sockfd = fd;
	}

	int init(int domain, int type, int protocol)
	{
		sockfd = ::socket(domain, type, protocol);
		return sockfd;
	}

	int connect(int family, const char* addr, int port)
	{
		sockaddr_in saddr;
		memset(&saddr, 0, sizeof(saddr));
  		
  		saddr.sin_family = family;
  		saddr.sin_addr.s_addr = inet_addr(addr);
   		saddr.sin_port = htons(port);

   		return ::connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
	};

	int bind(int family, int port)
	{
		sockaddr_in saddr;
		memset(&saddr, 0, sizeof(saddr));
  		
  		saddr.sin_family = family;
  		saddr.sin_addr.s_addr = INADDR_ANY;
   		saddr.sin_port = htons(port);

   		return ::bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
	};

	ssize_t send(const void *buf, size_t len, int flags)
	{
		return ::send(sockfd, buf, len, flags);
	}

    ssize_t recv(void *buf, size_t len, int flags)
    {
    	return ::recv(sockfd, buf, len, flags);
    }
};

class LinuxTcpSocket : public LinuxSocket
{
public:
	int tcp_nodelay()
	{
		int flag = 1;
		int ret = setsockopt( sockfd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag) );
		return ret;
	}
};

#endif