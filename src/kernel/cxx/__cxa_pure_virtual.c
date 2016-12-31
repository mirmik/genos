#include <kernel/panic.h>

void __cxa_pure_virtual() {
	while(1);
	panic("__cxa_pure_virtual");
}