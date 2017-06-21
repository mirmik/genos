#ifndef GENOS_DEVZERO_H
#define GENOS_DEVZERO_H

#include <fs/file.h>

namespace genos {
	class devzero : public genos::file {
		int write(const char* data, size_t size, size_t off) {
			return size;
		}

		int read(char* data, size_t size, size_t* off) {
			memset(data, 0, size);
			return size;
		}
		
		int open() override {}
		int release() override {}
	};
}


#endif