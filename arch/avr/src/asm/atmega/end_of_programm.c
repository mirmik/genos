#include <hal/irq.h>

void end_of_programm() {
	/*programm end stub*/
	global_irqs_disable();
	for( ; ; );
}