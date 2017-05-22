#ifndef GENOS_DATASTRUCT_ARGV_H
#define GENOS_DATASTRUCT_ARGV_H

#include "gxx/vector.h"
#include "gxx/ByteArray.h"
#include <assert.h>

class argvc_t {
	gxx::vector<char*> vect;

public:
	argvc_t(){}	

	//void internal_split(argvc_t* args, char* str, char dv = " ");
	void internal_split(char* str, char dv = ' ') {
		newarg_search:
		while(*str == dv) ++str;
		if (*str == 0) goto end;
	
		vect.push_back(str);
		while(*str != dv && *str != 0) ++str;
		if (*str == dv) { *str++ = 0; goto newarg_search; };

		end:
		return;
	};

	int argc() { return vect.size(); }
	char** argv() { return &vect[0]; }
};

struct str_argvc_t : public argvc_t {
	char* str;
	size_t sz;

	str_argvc_t(char* str) : str(str), sz(strlen(str)) {
		internal_split(str);
	}
};

#endif