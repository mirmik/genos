#include <nos/util/error.h>
#include <nos/dprint.h>

void nos::error(const char * msg) 
{
	dprln(msg);
	abort();
}
