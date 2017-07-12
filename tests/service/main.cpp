#include <hal/arch.h>
#include <gxx/debug/dprint.h>

int main() {
	arch_init();
	debug_print("HelloWorld");
}