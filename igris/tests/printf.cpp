#include <doctest/doctest.h>
#include <igris/util/printf_impl.h>
#include <string.h>

struct sprint_char_handler_data
{
    char *cursor;
};

static void sprint_printchar(void *d, int c)
{
    *(((struct sprint_char_handler_data *)d)->cursor)++ = c;
}

static int test_vsprintf(char *s, const char *format, va_list ap)
{
    int ret;

    struct sprint_char_handler_data data;
    data.cursor = s;

    ret = __printf(sprint_printchar, &data, format, ap);

    return ret;
}

static int test_sprintf(char *buf, const char *format, ...)
{
    int ret;
    va_list args;

    va_start(args, format);
    ret = test_vsprintf(buf, format, args);
    va_end(args);

    return ret;
}

/*static int test_snprintf(char *buf, size_t maxlen, const char *format, ...)
{
    int ret;
    va_list args;

    va_start(args, format);
    ret = test_vsprintf(buf, format, args);
    va_end(args);

    return ret;
}*/

static void test_prflt(char *buf, float fl)
{
    test_sprintf(buf, "%lf", fl);
}

static void test_prdbl(char *buf, double fl)
{
    test_sprintf(buf, "%lf", fl);
}

#include <stdio.h>
TEST_CASE("sprintf")
{
    char buf[48];
    memset(buf, 0, 48);
    test_sprintf(buf, "%d", 42);
    CHECK(strcmp("42", buf) == 0);

    test_sprintf(buf, "%ld", 42);
    CHECK(strcmp("42", buf) == 0);

    test_prflt(buf, 42.250);
    CHECK(strcmp("42.250000", buf) == 0);

    test_prdbl(buf, 42.250);
    CHECK(strcmp("42.250000", buf) == 0);
}