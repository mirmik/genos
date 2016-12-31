#include "debug/dprint.h"

//debug_putchar реализуется в основной программе.
	
void debug_write(const void * c, int i) {
	const char * chr = c; 
	while(i--) 
		debug_putchar(*chr++);
};
	