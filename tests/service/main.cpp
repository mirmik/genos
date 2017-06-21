#include <hal/arch.h>
#include <debug/dprint.h>

int main() {
	arch_init();
	debug_print("HelloWorld");
}