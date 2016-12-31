#ifndef GENOS_TCP_CLIENT_H
#define GENOS_TCP_CLIENT_H
   
#include <stdlib.h>    
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "intercom/socket.h"

class TcpClient : public LinuxTcpSocket
{

public:
	int init(char* address, int portno)
	{
		if (LinuxSocket::init(PF_INET, SOCK_STREAM, 0) < 0) return -1;
		if (connect(AF_INET, address, portno)) return -1;
	}
};

#endif