#ifndef GENOS_MESSANGER_TCP_SERV_H
#define GENOS_MESSANGER_TCP_SERV_H
    
#include <stdlib.h>    
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "intercom/socket.h"
#include "pthread.h"

class TcpServer : public LinuxTcpSocket
{
public:
	int init(int portno)
	{
		if (LinuxSocket::init(PF_INET, SOCK_STREAM, 0) < 0) return -1;
		if (bind(AF_INET, portno)) return -1;
	}

	int pthread_listen(int maxclnt, void*(*func)(void*))
	{
		pthread_t thr;
		int newsockfd;
		sockaddr_in caddr;
		socklen_t clen;

		::listen(sockfd, maxclnt);

		while(1)
		{
			newsockfd = ::accept(sockfd, (struct sockaddr *)&caddr, &clen);
	
  			if (newsockfd < 0) {
      			perror("ERROR on accept");
      			return -1;
   			};

   			pthread_create(&thr, 0, func, (void*)newsockfd);
   		}
	}
};

#endif