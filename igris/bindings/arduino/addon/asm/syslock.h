#ifndef IGRIS_ARDUINO_SYSLOCK_H
#define IGRIS_ARDUINO_SYSLOCK_H

#include <avr/interrupt.h>

static inline void system_lock(void)
{
    cli();
    __system_lock_state = 1;
}

static inline void system_unlock(void)
{
    __system_lock_state = 0;
    sei();
}

static inline unsigned char syslock_counter() { return system_lock_state(); }

#endif
