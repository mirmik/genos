#include <hal/irq.h>

///programm end stub
void end_of_programm() {
	irqs_disable();
	for( ; ; );
}