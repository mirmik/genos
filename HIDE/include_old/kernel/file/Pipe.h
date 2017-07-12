#ifndef GENOS_KERNEL_PIPE_H
#define GENOS_KERNEL_PIPE_H

#include <kernel/file/CharDev.h>

namespace Genos {
	class Pipe : public Genos::File {
	public:
		int32_t writeData(const char* data, size_t maxsz) {

		}

		int32_t readData(char* data, size_t maxsz) {

		}

		int32_t avail() { return -1; } 
		int32_t room() { return -1; }

		int open(uint16_t flags);
		int release();
	};
}

#endif