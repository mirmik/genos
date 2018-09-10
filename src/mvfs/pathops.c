#include <mvfs/pathops.h>
#include <stdlib.h>

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