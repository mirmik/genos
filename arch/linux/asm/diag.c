#include "kernel/diag.h"
#include "util/stub.h"

#include "stdio.h"

int terminal_diag_putc(int c)
{
	return putchar(c);
};

int terminal_diag_init()
{

};

struct diag_ops terminal_diag =
{
					terminal_diag_putc, 
	(diag_getc_t) 	do_nothing, 
					diag_write_stub, 
	(diag_read_t) 	do_nothing, 
					terminal_diag_init
};
