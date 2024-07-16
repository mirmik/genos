
// debug_putchar реализуется в основной программе.

#include <igris/dprint.h>

__attribute__((weak)) void debug_write(const char *c, int i)
{
    while (i--)
        debug_putchar(*c++);
}
