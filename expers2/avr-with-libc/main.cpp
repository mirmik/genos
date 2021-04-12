#include <genos/ktimer.h>
#include <avr/io.h>

int main() 
{
	DDRB = 0xFF;
	PORTB= 0xFF;
	while(1);
}