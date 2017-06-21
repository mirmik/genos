#ifndef GENOS_JOURNAL
#define GENOS_JOURNAL

#include <intypes.h>
#include <dlist_head.h>

struct journal {
	dlist_head list;
};

#endif