#ifndef GENOS_LOCATION_H
#define GENOS_LOCATION_H

#include <debug/dprint.h>

struct location {
	int line;
	const char* file;
	const char* func;
};

#define current_location()				\
({										\
	struct location __current_location;	\
	__current_location.line = __LINE__;	\
	__current_location.file = __FILE__;	\
	__current_location.func = __func__;	\
	__current_location;					\
})

static inline void debug_print_location(struct location location) {
	debug_print("file: "); debug_print(location.file); dln();
	debug_print("func: "); debug_print(location.func); dln();
	debug_print("line: "); debug_printdec_uint16(location.line); dln();
};

#endif /* UTIL_LOCATION_H_ */
