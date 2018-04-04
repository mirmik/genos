#ifndef GENOS_BANNER_H
#define GENOS_BANNER_H

#include <genos/iolib.h>

#define GENOS_VERSION "0.9.2"	

namespace genos {

	inline void print_banner(genos::file* f) {
		genos::print(f,
			"  __             __   __ \r\n"
			" /              /  | /   \r\n"
			"( __  ___  ___ (   |(___ \r\n"
			"|   )|___)|   )|   )    )\r\n"
			"|__/ |__  |  / |__/  __/ \r\n"
		);
	};
	
	inline void print_about(genos::file* f) {
		genos::print(f, "genos control system. version: ");
		genos::print(f, GENOS_VERSION);
	};

}

#endif