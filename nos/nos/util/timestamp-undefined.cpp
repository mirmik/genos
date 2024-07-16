#include <nos/timestamp.h>
#include <string.h>

void nos::timestamp(char * buffer, size_t maxsz) 
{
	strncpy(buffer, "UndTime", maxsz);
}
