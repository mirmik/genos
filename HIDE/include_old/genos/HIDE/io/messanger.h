#ifndef GENOS_MESSANGER_H
#define GENOS_MESSANGER_H

#include <genos/defines/size_t.h>
#include <assert.h>
#include <mem/allocator.h>

struct messanger
{
	virtual int send(char* buf, size_t len) = 0;
	virtual int recv(char* buf, size_t* len) = 0;
};

#endif