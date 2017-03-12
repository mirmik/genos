#ifndef KERNEL_DGRAMM_STUFFER_H
#define KERNEL_DGRAMM_STUFFER_H

#include <genos/sigslot/delegate.h>

class DatagrammStuffer {
	virtual int send(const char* data, size_t sz);
	delegate<void, const char*, size_t sz> received;
}

#endif