#ifndef GENOS_FILE_H
#define GENOS_FILE_H

#include <stdlib.h>

namespace genos {
	struct file {
	public:
		virtual bool is_cstream() { return false; }
		virtual int write(const char* data, size_t size, size_t off) = 0;
		virtual int read(char* data, size_t size, size_t* off) = 0;
		virtual void open() {};
		virtual void release() {};
	};
}

#endif