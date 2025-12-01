#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

void perror(const char *s) {
	printf("%s: %s", s, strerror(errno));
}
