#include "local_wait.h"

int waiter_get(struct waiter ** wptr) 
{
	*wptr = (struct waiter *) malloc(sizeof(struct waiter));
	return 0;
}

int waiter_put(struct waiter * w) 
{
	free(w);
	return 0;
}