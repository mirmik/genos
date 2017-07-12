#include <gxx/debug/dprint.h>
#include <kernel/diag.h>

void debug_putchar(char c) 
{
	diag_putc(c);
};

void debug_write(const void*c, int i)
{
	diag_write(c, i);
};
	
	
	