#include <hal/board.h>
#include <hal/irq.h>

#include <systime/systime.h>
#include <gxx/debug/dprint.h>

int main() {
	board_init();

	irqs_enable();
	while(1) {
		debug_printdec_uint64(millis());
		debug_print_line("");
		delay(1000);
	}
}