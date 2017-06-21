#include "mem/allocator.h"
#include "stdlib.h"

#include "debug/dprint.h"

struct allocator_operations malloc_ops = {malloc, free, realloc};

void* ctrlmalloc (size_t sz) {
	void * ret = malloc(sz);
	debug_print("MEMORY malloc\t");debug_printdec_uint32(sz);
	debug_print("\t\tret ");debug_printhex_ptr(ret);  dln(); 
	return ret;
};

void ctrlfree (void* ptr) {
	free(ptr);
	debug_print("MEMORY free\t"); debug_printhex_ptr(ptr); dln();
};

void* ctrlrealloc (void* ptr, size_t sz) {
	void * ret = realloc(ptr,sz);
	debug_print("MEMORY realloc\t");debug_printdec_uint32(sz); 
	debug_print(", ");debug_printhex_ptr(ptr);
	debug_print("\tret ");debug_printhex_ptr(ret);  dln(); 
	return ret;
};

struct allocator_operations ctrl_malloc_ops = {ctrlmalloc, ctrlfree, ctrlrealloc};