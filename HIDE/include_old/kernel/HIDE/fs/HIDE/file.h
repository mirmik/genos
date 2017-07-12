#ifndef KERNEL_NAMESPACE_FILE_H
#define KERNEL_NAMESPACE_FILE_H

#include <inttypes.h>
#include <gxx/debug/dprint.h>

#include <gxx/ByteArray.h>

namespace Kernel {

	class dirent;

	class File {
	public:
		uint8_t 	flags;

		virtual int close() = 0;
		virtual int write(const char* data, int length, uint8_t opt) = 0;
		virtual int read(char* data, int length, uint8_t opt) = 0;
		virtual Kernel::dirent* readdir(char* data, int lmax) = 0;
	};
};

#endif