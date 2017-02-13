#ifndef GENOS_FILE_H
#define GENOS_FILE_H

#include <proto/print.h>
#include <kernel/event/event.h>

class File {
public:
	virtual int fwrite(const char* data, size_t size) = 0;
	virtual int fread(char* data, size_t size) = 0;
};

class CharDevFile : public Print, public File {
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