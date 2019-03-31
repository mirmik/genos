#include <diag/diag.h>

void debug_putchar(char c)  {
	diag_putc(c);
}

void debug_write(const char* c, int i) {
	diag_write(c, i);
}