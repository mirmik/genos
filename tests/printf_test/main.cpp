#include <hal/board.h>
#include <gxx/debug/dprint.h>

int main() {
	board_init();
	dprf("HelloWorld %d\n", 512);

	while(1);
}