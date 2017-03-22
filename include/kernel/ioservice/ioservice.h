#ifndef IO_SERVICE_STREAM_H
#define IO_SERVICE_STREAM_H

#include <kernel/sched/automschedee.h>
#include <compiler.h>

struct ioservice;

struct ioservice_operations {
	int(*io_write)(struct ioservice*, const char* data, size_t sz);
	int(*io_write)(struct ioservice*, const char* data, size_t sz);
	int(*io_read)(struct ioservice*, const char* data, size_t sz);
};

struct ioservice {
	ioservice_operations ioops;
	void * privdata;	
};

__BEGIN_DECLS

static void construct_ioservice() {}

__END_DECLS

#endif