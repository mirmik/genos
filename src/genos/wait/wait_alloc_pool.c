#include "local_wait.h"
#include <igris/datastruct/pool.h>

#define WAIT_POOL_SIZE 10;

struct pool_head wait_pool;
char wait_pool_buffer[sizeof(struct waiter) * WAIT_POOL_SIZE];

int wait_allocator_init() 
{
	pool_init(&wait_pool);
	pool_engage(&wait_pool, wait_pool_buffer);
}

int waiter_get(struct waiter ** wptr) 
{
	*wptr = (struct waiter *) pool_alloc(&wait_pool);
	
	if (*wptr == NULL)
		return -1; 

	return 0;
}

int waiter_put(struct waiter * w) 
{
	pool_free(w);
	return 0;
}