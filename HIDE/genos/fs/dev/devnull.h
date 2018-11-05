#ifndef GENOS_DEVNULL_H
#define GENOS_DEVNULL_H

#include <fs/file.h>

namespace genos {
	class devnull : public genos::file {
		int write(const char* data, size_t size, size_t off) {
			return size;
		}

		int read(char* data, size_t size, size_t* off) {
			return size;
		}

		int open() override {}
		int release() override {}
	};
}


#endif