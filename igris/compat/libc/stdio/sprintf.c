#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include <igris/util/printf_impl.h>

struct sprint_char_handler_data
{
    char *cursor;
};

static void sprint_printchar(void *d, int c)
{
    *(((struct sprint_char_handler_data *)d)->cursor)++ = c;
}

int vsprintf(char *s, const char *format, va_list ap)
{
    int ret;

    struct sprint_char_handler_data data;
    data.cursor = s;

    ret = __printf(sprint_printchar, &data, format, ap);
    *data.cursor = 0;    

    return ret;
}

int sprintf(char *buf, const char *format, ...)
{
    int ret;
    va_list args;

    va_start(args, format);
    ret = vsprintf(buf, format, args);
    va_end(args);

    return ret;
}

int snprintf(char *buf, size_t maxlen, const char *format, ...)
{
    (void) maxlen; //TODO
    int ret;
    va_list args;

    va_start(args, format);
    ret = vsprintf(buf, format, args);
    va_end(args);

    return ret;
}
