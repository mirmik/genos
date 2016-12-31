/**
 * @file
 *
 * @date Aug 28, 2013
 * @author: Anton Bondarev
 */
#include <stdlib.h>
#include <string.h>
#include <math.h>

char *strndup(const char *s, size_t size) {
	char *ret;
	size_t len = min(strlen(s), size);

	ret = malloc(len + 1);
	if (ret == NULL) {
		return NULL;
	}

	strncpy(ret, s, len);
	ret[len] = '\0';

	return ret;
}
