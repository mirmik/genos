#ifndef TCPSMSG_H
#define TCPSMSG_H

#include <genos/io/messanger.h>

struct tcpsmsg : public messanger
{
	struct allocatr_ops* alloc;

	int send(char* buf, size_t len)
	{

	};

	int recv(char* buf, size_t* len)
	{

	};

	int available()
	{
		return -1;
	};

	size_t sznext()
	{
		return -1;
	};
};

#endif