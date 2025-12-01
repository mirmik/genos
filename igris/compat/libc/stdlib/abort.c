#include <igris/dprint/dprint.h>

void abort(void) {
	debug_print("abort");
	while(1);
}
