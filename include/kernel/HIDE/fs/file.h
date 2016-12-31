#ifndef GENOS_KERNEL_FILE_H
#define GENOS_KERNEL_FILE_H

#include <defines/off_t.h>
#include <defines/size_t.h>
#include <defines/ssize_t.h>

#include <proto/stream.h>

class File : public Stream {

public:

	enum Type {
		ftype_tty,
		ftype_pipe,		
	};

	Type type;
	
	virtual int open();	
	virtual int close();

	virtual int write(const char* str, size_t len);
	virtual int read(char* str, size_t len);

	virtual off_t seek(off_t offset, int whence);
	//virtual int ioctl(int cmd, unsigned long arg);

};

#endif