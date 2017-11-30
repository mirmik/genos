#include <gxx/debug/delay.h>
#include <gxx/util/bits.h>

#include <periph/map.h>
#include <arch/gpio.h>

int del = 50000;

arch::gpio::pin led(GPIOA, 17);
	
int main() {
	led.mode(1);

	while(1) {
		debug_simple_delay(del);
		led.tgl();
	}

}