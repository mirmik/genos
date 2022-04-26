#include <igris/util/bug.h>

int _read() 
{
	BUG();
}

int _write() 
{
	BUG();
}

int _lseek() 
{
	BUG();
}

int _isatty() 
{
	BUG();
}

int _fstat() 
{
	BUG();
}

int _close(int fd) 
{
	BUG();
}

int _sbrk(int fd) 
{
	BUG();
}

int _getpid() 
{
	BUG();
}

int _kill() 
{
	BUG();
}

int _exit() 
{
	BUG();
}