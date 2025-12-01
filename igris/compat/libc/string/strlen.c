/**
 * @file
 * @brief Implementation of #strlen() function.
 *
 * @date 23.11.09
 * @author Nikolay Korotky
 */

#include <string.h>

__attribute__((weak))
size_t strlen(const char *str) {
	const char *s = str;

	while (*s++)
		;

	return (size_t) (s - str - 1);
}
