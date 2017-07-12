#ifndef GENOS_STREAM_DEVICE_H
#define GENOS_STREAM_DEVICE_H

#include <datastruct/ring_head.h>

struct strmdevice;

struct strmdevice_operations {
	int(*io_write)(struct strmdevice*, const char* data, size_t sz);
	int(*io_read)(struct strmdevice*, const char* data, size_t sz);
};

struct strmdevice {
	const strmdev_operations * strmops;

	struct ring_head 
};

#endif