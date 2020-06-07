#include <hal/board.h>
#include <systime/systime.h>

#include <hal/irq.h>
#include <util/cpu_delay.h>

#include <asm/stm32_pll.h>
#include <asm/stm32_systick.h>

int main() 
{
	board_init(1);
	stm32_pll_setup(6,85,2,2,2);
	stm32_systick_config(170000);
	stm32_clockbus_enable_pll_mode(170000000);

	board::sysuart.setup(115200, 'n', 8, 1);

	irqs_enable();

	board::sysled.set(0);
		
	while(1) 
	{
		board::sysled.toggle();
		dprln("HelloWorld");
		delay(1000);
	}
}
