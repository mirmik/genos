#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

/*int vprintf(const char *format, va_list args) {
	return vfprintf(stdout, format, args);
}

int printf(const char *format, ...) 
{
	int ret;
	va_list args;

	va_start(args, format);
	ret = vfdprintf(1, format, args);
	va_end(args);

	return ret;
}*/
