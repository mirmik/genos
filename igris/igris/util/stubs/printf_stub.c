#include <igris/util/printf_impl.h>

int __printf(void (*printchar_handler)(void *d, int c), void *printchar_data,
             const char *format, va_list args) {}