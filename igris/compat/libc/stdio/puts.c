#include <stdio.h>
#include <unistd.h>
#include <string.h>

int puts(const char * str) 
{
	char n = '\n';
	int ret = write(1, str, strlen(str));
	ret += write(1, &n, 1);
	return ret;
}
