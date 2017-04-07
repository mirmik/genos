#ifndef GENOS_BANNER_H
#define GENOS_BANNER_H

#include <kernel/devices/serial/Stream.h>

#define GENOS_VERSION "0.9.1"	

static void print_banner(Genos::Stream& f) {
	f.println();
	f.println("  __             __   __ ");
	f.println(" /              /  | /   ");
	f.println("( __  ___  ___ (   |(___ ");
	f.println("|   )|___)|   )|   )    )");
	f.println("|__/ |__  |  / |__/  __/ ");
};

static void print_about(Genos::Stream& f) {
	f.print("GenOS control system. version: ");
	f.println(GENOS_VERSION);
};

#endif