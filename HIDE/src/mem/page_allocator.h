#ifndef GENOS_PAGE_ALLOCATOR_H
#define GENOS_PAGE_ALLOCATOR_H

struct page_allocator 
{
	char * heap_start;
	char * heap_end;

	char * brk;

	slist_head * freelist;
};

struct 

#endif