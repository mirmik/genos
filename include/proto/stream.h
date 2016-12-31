#ifndef GENOS_STREAM
#define GENOS_STREAM

#include <proto/print.h>

class Stream : public Print {

public:
	virtual int read(char* str, size_t len) = 0;

	char getc() {
		char c;
		int ret = read(&c, 1);
		if (ret != 1) return -1;
		return c;	
	}

};

#endif