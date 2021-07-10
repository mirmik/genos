#include <avr/io.h>

.global debug_blink

debug_blink:

ldi r16, 0xFF
ldi r17, 0x00
out _SFR_IO_ADDR(DDRB), r16
out _SFR_IO_ADDR(PORTB), r17

ldi r18, 0xFF
ldi r19, 0xFF

sloop:
ldi r20, 0x10
loop:
dec r18
cpi r18, 0
brne loop
dec r19
cpi r19, 0
brne loop
dec r20
cpi r20, 0
brne loop

out _SFR_IO_ADDR(PORTB), r17

sloop1:
ldi r20, 0x10
loop1:
dec r18
cpi r18, 0
brne loop1
dec r19
cpi r19, 0
brne loop1
dec r20
cpi r20, 0
brne loop1

out _SFR_IO_ADDR(PORTB), r16

rjmp sloop