#ifndef GENOS_STREAM
#define GENOS_STREAM

#include <kernel/devices/serial/Print.h>

namespace Genos {
	class Stream : public Print {
	
	public:
		virtual int read(char* str, size_t len) = 0;
	
		char getc() {
			char c;
			int ret = read(&c, 1);
			if (ret != 1) return -1;
			return c;	
		}
	
		virtual int avail() = 0;
		virtual int room() = 0;
	
	};
}

#endif