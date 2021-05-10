#include <avr/io.h>

.org 0x0000      
rjmp Reset              

Reset:
	ldi r16, 0x00
	out DDRB, r16

	ldi r16, 0x00
	out PORTB, r16

	rjmp Reset

main:
	rjmp Reset
