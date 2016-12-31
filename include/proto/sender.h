#ifndef GENOS_SENDER_H
#define GENOS_SENDER_H

#include <stdlib.h>

class PackageSender {
	virtual void send(void* data, int size);
};

#endif