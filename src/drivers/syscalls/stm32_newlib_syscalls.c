#include <asm/irq.h>
#include <igris/util/bug.h>

void _exit(int code) 
{
	(void) code;

	irqs_disable();
	while(1);
} 

void _kill() 
{
	BUG();
}

void _sbrk() 
{
	BUG();
}

void _read() 
{
	BUG();
}

void _write() 
{
	BUG();
}

void _lseek() 
{
	BUG();
}

void _isatty() 
{
	BUG();
}

void _close() 
{
	BUG();
}

void _open() 
{
	BUG();
}

void _getpid() 
{
	BUG();
}

void _fstat() 
{
	BUG();
}