#include "kernel/panic.h"
#include "debug/dprint.h"

void end_of_programm() {
	panic("END_OF_PROGRAMM\r\n");
};