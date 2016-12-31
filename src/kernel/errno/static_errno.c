#include "errno.h"

int _errno = 0;

int* __errno_location()
{
	return & _errno;
};