#include <kernel/pipe/pipe.h>

class RingPipe : public FileAbstract {
	int write(const char* data, size_t size);
	int read(const char* data, size_t size);

	int avail();
	int room();

	int flush();
};