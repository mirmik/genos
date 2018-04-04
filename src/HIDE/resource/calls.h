#ifndef GXX_CALLS_H
#define GXX_CALLS_H

#include <inttypes.h>
#include <stdlib.h>

#include <fs/file.h>

namespace genos {

	int open(const char* path, uint16_t flags);
	int openptr(genos::file* path, uint16_t flags);

	int close(int fd);

	int write(int fd, const char* path, size_t size);
	int read(int fd, char* path, size_t size);

	int fcntl(int fd, int cmd, ... /* arg */ );

	int pipe();

}

#endif