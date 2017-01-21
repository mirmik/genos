#include <hal/irq.h>

void end_of_programm() {
	/*programm end stub*/
	global_irq_disable();
	for( ; ; );
}