#ifndef GENOS_FILE_H
#define GENOS_FILE_H

#include <proto/stream.h>
#include <kernel/event/event.h>
#include <kernel/ns/NameSpace.h>

class File {
public:
	virtual int fwrite(const char* data, size_t size) = 0;
	virtual int fread(char* data, size_t size) = 0;

	virtual int open() {}
	virtual int release() {}
};

class KernelFile : public File {};

class CharDevFile : public Stream, public KernelFile {
public:
	event_signal rxevent;
	event_signal txevent;

public:
	virtual int getc() {
		char c;
		read(&c, 1);
		return c;
	}

	virtual int fwrite(const char* data, size_t size);
	virtual int fread(char* data, size_t size);

	virtual int write(const char* data, size_t size) = 0;
	virtual int read(char* data, size_t size) = 0;

	virtual int avail() = 0;
	virtual int room() = 0;

	virtual int flush() = 0;
};

#endif