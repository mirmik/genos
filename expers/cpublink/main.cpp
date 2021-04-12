#include <hal/board.h>
#include <systime/systime.h>

#include <hal/irq.h>
#include <util/cpu_delay.h>

int main() 
{
	board_init();
	blink_test_cpudelay(500000);
}
