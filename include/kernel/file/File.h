#ifndef GENOS_FILE_H
#define GENOS_FILE_H

namespace Genos {
	enum FileFlags {
		Locked = 0x0001;
	}

	class File {
		uint16_t flags;

		virtual int32_t write(const char* data, size_t maxsz, uint16_t flags) = 0;
		virtual int32_t read(char* data, size_t maxsz, uint16_t flags) = 0;

		//virtual int8_t putchar(char c) = 0;
		//virtual int8_t getchar() = 0;

		virtual int32_t avail() { return -1; } 
		virtual int32_t room() { return -1; }

		virtual int open();
		virtual int release();
	};
}

#endif