#ifndef KERNEL_DEVNULL_H
#define KERNEL_DEVNULL_H

#include <kernel/fs/chardev.h>
#include <string.h>

class DevNull : public Kernel::CharDev {

	int write(const char* data, int length, uint8_t opt) {
		return length;
	}

	int read(char* data, int length, uint8_t opt) {
		memset(data, length, 0);
		return length;
	}

	int close() {
		return 0;
	}

	Kernel::dirent* readdir(char*, int) {
		return nullptr;
	};

};

extern DevNull devnull;

#endif