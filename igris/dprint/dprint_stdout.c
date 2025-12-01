#include <stdio.h>
#include <stdlib.h>

#ifdef _MSC_VER

void debug_putchar(char c)
{
    size_t _ = fwrite(&c, sizeof(char), 1, stdout);
    (void)_;
}

void debug_write(const char *c, int i)
{
    size_t _ = fwrite(c, sizeof(char), i, stdout);
    (void)_;
}

#else

#include <unistd.h>

#define GXX_DEBUG_STDOUT STDOUT_FILENO

void debug_putchar(char c)
{
    int _ = write(GXX_DEBUG_STDOUT, &c, 1);
    (void)_;
}

void debug_write(const char *c, int i)
{
    int _ = write(GXX_DEBUG_STDOUT, c, i);
    (void)_;
}

#endif
