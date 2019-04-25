#include <errno.h>

static int idle_errno = 0;
static int * ____errno_location = &idle_errno;

int * __errno_location() {
	return ____errno_location;
}

int * set_errno_location(int * new) {
	int * old = ____errno_location;
	____errno_location = new;
	return old;
}