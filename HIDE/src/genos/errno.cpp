#include <errno.h>
#include <genos/schedee.h>

int* __errno_location() 
{
	static int global_errno;

	if (current_schedee() == nullptr) 
	{
		return &global_errno;
	}

	return &current_schedee()->local_errno;
}