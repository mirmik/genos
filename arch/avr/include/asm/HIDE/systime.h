#ifndef AVR_SYSTIME_H
#define AVR_SYSTIME_H

struct TimeStamp {
	uint64_t jiffies;
	uint8_t count;
};

#endif