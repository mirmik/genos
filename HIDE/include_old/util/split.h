#ifndef GENOS_UTILXX_SPLIT_H
#define GENOS_UTILXX_SPLIT_H

#include <genos/datastruct/argvc.h>

inline static void internal_split(argvc_t* args, char* str, char dv = ' ');

inline static void internal_split(argvc_t* args, char* str, char dv)
{
	args->c = 0;

	newarg_search:
	while(*str == dv) ++str;
	if (*str == 0) goto end;
	
	args->v[args->c++] = str;
	while(*str != dv && *str != 0) ++str;
	if (*str == dv) { *str++ = 0; goto newarg_search; };

	end:
	return;
};

#endif