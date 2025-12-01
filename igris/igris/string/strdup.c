#include <igris/string/igris_string.h>

#include <stdlib.h>
#include <stdint.h>

char * igris_strdup(const char *s) 
{
	size_t len = igris_strlen(s);
	char * ptr = malloc(len);
	memcpy(ptr, s, len);
	return ptr;
}

size_t igris_strlen(const char *str) 
{
	const char *s = str;
	while (*s++);
	return (size_t) (s - str - 1);
}
