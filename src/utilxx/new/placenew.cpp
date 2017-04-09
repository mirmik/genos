#include <utilxx/new.h>

void * operator new(size_t, void *ptr) noexcept {
	return ptr;
};