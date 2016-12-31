#ifndef GENOS_FILE_H
#define GENOS_FILE_H

#include <proto/print.h>

class FileAbstract : public Print {
public:
	char getc() {
		char c;
		read(&c, 1);
		return c;
	}

	virtual int write(const char* data, size_t size) = 0;
	virtual int read(char* data, size_t size) = 0;
	
	virtual int avail() = 0;
	virtual int room() = 0;

	virtual int flush() = 0;
};

#endif