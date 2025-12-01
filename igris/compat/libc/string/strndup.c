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
	size_t slen = strlen(s);
	size_t len = slen < size ? slen : size;

	ret = malloc(len + 1);
	if (ret == NULL) {
		return NULL;
	}

	memcpy(ret, s, len);
	ret[len] = '\0';

	return ret;
}
