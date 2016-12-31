#ifndef DATASTRUCT_LBUF_H
#define DATASTRUCT_LBUF_H

#include <stdlib.h>

struct lbuf {
	void* buf;
	size_t len;
};

#endif