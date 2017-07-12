#ifndef GENOS_LIN_MALLOC_H
#define GENOS_LIN_MALLOC_H

struct __freelist {
	size_t sz;
	struct __freelist *nx;
};

#endif