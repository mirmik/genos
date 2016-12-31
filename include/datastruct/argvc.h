#ifndef GENOS_DATASTRUCT_ARGV_H
#define GENOS_DATASTRUCT_ARGV_H

#include "gxx/ByteArray.h"
#include <assert.h>

struct argvc_t {
	int c;
	char** v;

	argvc_t() : v(nullptr), c(0) {}	

	gxx::string to_str()
	{
		gxx::string str;
		str.reserve(128);
		str << "total=" << c;
		str << "[";
		for(int i = 0; i < c; i++)
			str << gxx::string(v[i]) << gxx::string(",");
		str << "]";
		return str;
	};

	//void internal_split(argvc_t* args, char* str, char dv = " ");
	void internal_split(char* str, char dv = ' ') {
		assert(v);
		c = 0;

		newarg_search:
		while(*str == dv) ++str;
		if (*str == 0) goto end;
	
		v[c++] = str;
		while(*str != dv && *str != 0) ++str;
		if (*str == dv) { *str++ = 0; goto newarg_search; };

		end:
		return;
	};

};
#endif