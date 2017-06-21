#ifndef GENOS_BANNER_H
#define GENOS_BANNER_H

#include <kernel/devices/serial/Stream.h>

#define GENOS_VERSION "0.9.1"	

static void print_banner(Genos::Stream& f) {
	f.print(
				"  __             __   __ \r\n"
				" /              /  | /   \r\n"
				"( __  ___  ___ (   |(___ \r\n"
				"|   )|___)|   )|   )    )\r\n"
				"|__/ |__  |  / |__/  __/ \r\n"
											);
};

static void print_about(Genos::Stream& f) {
	f.print("GenOS control system. version: ");
	f.println(GENOS_VERSION);
};

#endif