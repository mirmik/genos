#include <utilxx/new.h>
#include <mem/sysalloc.h>

void * operator new(size_t, void *ptr) noexcept {
	return ptr;
};

void * operator new(size_t sz) noexcept {
	return sysalloc(sz);
};

void operator delete(void* ptr) noexcept {
	return sysfree(ptr);
};