#ifndef GENOS_BANNER_H
#define GENOS_BANNER_H

#include <gxx/io/ostream.h>

#define GENOS_VERSION "0.9.1"	

namespace genos {

	inline void print_banner(gxx::io::ostream& f) {
		f.print(
			"  __             __   __ \r\n"
			" /              /  | /   \r\n"
			"( __  ___  ___ (   |(___ \r\n"
			"|   )|___)|   )|   )    )\r\n"
			"|__/ |__  |  / |__/  __/ \r\n"
		);
	};
	
	inline void print_about(gxx::io::ostream& f) {
		f.print("genos control system. version: ");
		f.println(GENOS_VERSION);
	};

}

#endif