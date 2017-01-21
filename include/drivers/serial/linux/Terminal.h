#ifndef GENOS_LINUX_TERMINAL_H
#define GENOS_LINUX_TERMINAL_H

#include <stdio.h>
#include <unistd.h>
#include <kernel/file/file.h>

class LinuxTerminal : public FileAbstract {
	int write(const char* data, size_t size) {
		return ::write(1, data, size);
	}
	
	int read(char* data, size_t size) {
		return ::read(0, data, size);
	}

	int avail() { 
		return INT_MAX; 
	}
	
	int room() { 
		return INT_MAX; 
	}

	int flush() {
		return 0;
	}
};

#endif