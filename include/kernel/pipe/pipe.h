#ifndef GENOS_PIPES_H
#define GENOS_PIPES_H

#include <kernel/file/file.h>
#include <utilxx/classes/ByteRing.h>

class Pipe : public FileAbstract {
	virtual int write(const char* data, size_t size) = 0;
	virtual int read(const char* data, size_t size) = 0;
	
	virtual int avail() = 0;
	virtual int room() = 0;

	virtual int flush() = 0;
};

class RingPipe : public BasicPipe {
	ByteRing 

	int write(const char* data, size_t size);
	int read(const char* data, size_t size);
	
	int avail();
	int room();

	int flush();
};

#endif