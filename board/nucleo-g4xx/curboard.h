#ifndef NUCLEO_BOARDS
#define NUCLEO_BOARDS

#include <sys/cdefs.h>
#include <drivers/gpio/pin.h>

#define HSE_FREQ 24000000

#define SYSLED_GPIO GPIOA
#define SYSLED_PINMASK (1<<5)

namespace board 
{
	extern gpio_pin sysled;
}

__BEGIN_DECLS

void board_init();

__END_DECLS

#endif