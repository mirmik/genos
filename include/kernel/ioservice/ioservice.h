#ifndef IO_SERVICE_STREAM_H
#define IO_SERVICE_STREAM_H

#include <kernel/sched/automschedee.h>
#include <kernel/file/file.h>

struct ioservice;

struct ioservice_operations {
	int(*io_write)(struct ioservice*, const char* data, size_t sz);
	int(*io_read)(struct ioservice*, const char* data, size_t sz);
};

struct ioservice : public autom_schedee {
	ioservice_operations ioops;
	
	void Routine() override {

	}
};

#endif