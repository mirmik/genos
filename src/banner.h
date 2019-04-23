#ifndef GENOS_BANNER_H
#define GENOS_BANNER_H

#include <igris/dprint.h>

//#include <genos/iolib.h>

//#define GENOS_VERSION "0.10.1"	

/*static inline void print_banner(genos::file* f) {
	genos::print(f,
		"  __             __   __ \r\n"
		" /              /  | /   \r\n"
		"( __  ___  ___ (   |(___ \r\n"
		"|   )|___)|   )|   )    )\r\n"
		"|__/ |__  |  / |__/  __/ \r\n"
	);
};
	
static inline void print_about(genos::file* f) {
	genos::print(f, "genos control system. version: ");
	genos::print(f, GENOS_VERSION);
};*/

static inline 
void debug_print_banner() {
	debug_print(
		"  __             __   __ \r\n"
		" /              /  | /   \r\n"
		"( __  ___  ___ (   |(___ \r\n"
		"|   )|___)|   )|   )    )\r\n"
		"|__/ |__  |  / |__/  __/ \r\n"
	);
};

#endif