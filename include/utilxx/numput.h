#ifndef UTILXX_NUMPUT
#define UTILXX_NUMPUT

#include <inttypes.h>
#include <util/math.h>

template <typename Integer>
char* __backward_numput_positive_simple(char* ptr, Integer num, uint8_t base) {
	if (num == 0) {
		*ptr-- = '0';
		return ptr;
	}

	for (; num != 0; num /= base) {
		uint8_t mod = num % base;
		*ptr-- = (mod < 10) ? (mod + '0') : (mod - 10 + 'A');
	}

	return ptr;
};

template <typename Integer>
char* __backward_numput_simple(char* ptr, Integer num, uint8_t base) {
	bool neg = num < 0;
	ptr = __backward_numput_positive_simple(ptr, abs(num), base);
	if (neg) *ptr-- = '-';
	return ptr;
};

template <typename Integer>
char* backward_numput_simple(char* buf, size_t len, Integer num, uint8_t base) {
	return __backward_numput_simple(buf + len - 1, num, base) + 1;
}


#endif