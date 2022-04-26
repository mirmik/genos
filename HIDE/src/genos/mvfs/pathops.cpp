#include <mvfs/pathops.h>
#include <stdlib.h>
#include <stdbool.h>

int path_is_single_dot(const char *path) {
	char nc = *(path + 1);

	return *path == '.' && (nc == '/' || nc == '\0');
}

const char *path_next(const char *path, unsigned int *p_len) {

	if (!path) {
		return NULL;
	}

	// Skip leading slashes.
	// XXX Skip single dots too
	while (*path == '/' || path_is_single_dot(path)) {
		++path;
	}

	if (!*path) {
		return NULL;
	}

	if (p_len) {
		const char *end = path;

		while (*end && *end != '/') {
			++end;
		}

		*p_len = end - path;
	}

	return path;
}

int path_is_double_dot(const char *path) {
	return *path == '.' && *(path + 1) == '.'
		&& (*(path + 2) == '/' || *(path + 2) == '\0');
}

int path_is_abs(const char *path) {
	return path[0] == '/';  
}

int path_is_simple(const char *path) {
	char c;
	//for(c = *path++; c; c = *path++) 
	while((c = *path++)) 
		if (c == '/')
			return false;
	return true;
}