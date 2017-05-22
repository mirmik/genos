#ifndef GENOS_FILE_H
#define GENOS_FILE_H

#include <util/nosuported_abort.h>
#include <inttypes.h>
#include <kernel/devices/serial/Stream.h>

namespace Genos {
	class File {
	public:
		virtual int write(const char* str, size_t sz) { nosuported_abort(); }
		virtual int read(char* str, size_t sz) { nosuported_abort(); }
		virtual int seek(size_t sz) { nosuported_abort(); }

		virtual void open() { nosuported_abort(); }
		virtual void release() { nosuported_abort(); }
	};

	
	class StreamFile : File {
		Stream * strm;

	public:
		StreamFile(Stream* strm) : strm(strm) {}

		int write(const char* str, size_t sz) override { return strm->write(str, sz); }
		int read(char* str, size_t sz) override { return strm->read(str, sz); }
		int seek(size_t sz) override { return -1; }

		void open() override { dprln("StreamFile open"); }
		void release() override { dprln("StreamFile release"); }

		static StreamFile create(Stream* strm);
	};
}

#endif