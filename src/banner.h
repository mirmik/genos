#ifndef GENOS_BANNER_H
#define GENOS_BANNER_H

#include <igris/dprint.h>
#include <genos/cdev.h>

static const char* banner =
    "  __             __   __ \r\n"
    " /              /  | /   \r\n"
    "( __  ___  ___ (   |(___ \r\n"
    "|   )|___)|   )|   )    )\r\n"
    "|__/ |__  |  / |__/  __/ \r\n";

static inline void cdev_print_banner(genos::char_device* f, int flags = 0)
{
	f->print(banner, flags);
};

static inline void print_genos_greetings(genos::char_device* f, int flags = 0)
{
	f->print("\r\n", flags);
	f->print("\r\n", flags);
	f->print(banner, flags);
	f->print("Powered by GenOS\r\n", flags);
	f->print("\r\n", flags);
};

static inline
void debug_print_banner()
{
	debug_print(banner);
};

#endif