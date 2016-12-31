#ifndef EVICLASS_H
#define EVICLASS_H

#include "genos/datastruct/dlist_head.h"
#include "gxx/container/dictionary.h"

class evident_class {
	dlist_head lnk;
	const char* name;

	//void reg_method(const char* name, evident);
};

struct evident_method {
	uint8_t totalargs;
};

/*
int printf(const char *format, ...) {
	int ret;
	va_list args;

	assert(format != NULL);

	va_start(args, format);
	ret = vprintf(format, args);
	va_end(args);

	return ret;
}*/

#endif