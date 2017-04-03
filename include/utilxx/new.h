#ifndef PLACEMENT_NEW_H
#define PLACEMENT_NEW_H

#include "defines/size_t.h"

void * operator new(size_t, void *ptr) noexcept;
void * operator new(size_t) noexcept;
void operator delete(void*) noexcept __attribute__((__externally_visible__));

//template<typename Allocator> void * operator new(size_t, Allocator& alloc);
//template<typename Allocator> void operator delete(void*, Allocator& alloc);

#endif