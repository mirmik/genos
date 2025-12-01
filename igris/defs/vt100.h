#ifndef IGRIS_VT100_DEFS_H
#define IGRIS_VT100_DEFS_H

#include <igris/compiler.h>
#include <igris/util/numconvert.h>

#define VT100_ERASE_LINE_AFTER_CURSOR "\x1B[K"

#define VT100_UP "\x1B[A"
#define VT100_DOWN "\x1B[B"
#define VT100_RIGHT "\x1B[C"
#define VT100_LEFT "\x1B[D"

__BEGIN_DECLS

static inline int vt100_left(char *buf, int arg)
{
    char *eptr;

    buf[0] = '\x1B';
    buf[1] = '[';
    eptr = igris_i32toa(arg, buf + 2, 10);
    *eptr = 'D';
    *(eptr + 1) = 0;
    return (int)(eptr - buf) + 1;
}

__END_DECLS

#endif
