#ifndef GENOS_DEVNULL_H
#define GENOS_DEVNULL_H

#include <limits.h>
#include <kernel/file/file.h>

class DevNull : public FileAbstract {
	int write(const char* data, size_t size) {
		return size;
	}
	
	int read(char* data, size_t size) {
		return 0;
	}

	int avail() { 
		return 0; 
	}
	
	int room() { 
		return INT_MAX; 
	}

	int flush() {
		return 0;
	}
};

#endif 