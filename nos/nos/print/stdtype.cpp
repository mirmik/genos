#include <nos/io/ostream.h>
#include <nos/print/stdtype.h>
#include <nos/util/numconvert.h>

int nos_print(nos::ostream &out, const char *str)
{
    return *out.write(str, strlen(str));
}

int nos_print(nos::ostream &out, char *str)
{
    return *out.write(str, strlen(str));
}

#if __has_include(<string_view>)
int nos_print(nos::ostream &out, const std::string_view &buf)
{
    return *out.write(buf.data(), buf.size());
}
#endif

int nos_print(nos::ostream &out, bool obj)
{
    return nos_print(out, obj ? "true" : "false");
}

int nos_print(nos::ostream &out, signed char obj)
{
    char buf[48];
    __nos_itoa(obj, buf, 10);
    return nos_print(out, buf);
}
int nos_print(nos::ostream &out, signed short obj)
{
    char buf[48];
    __nos_itoa(obj, buf, 10);
    return nos_print(out, buf);
}
int nos_print(nos::ostream &out, signed int obj)
{
    char buf[48];
    __nos_itoa(obj, buf, 10);
    return nos_print(out, buf);
}

int nos_print(nos::ostream &out, signed long obj)
{
    char buf[48];
    __nos_itoa(obj, buf, 10);
    return nos_print(out, buf);
}
int nos_print(nos::ostream &out, signed long long obj)
{
    char buf[48];
    __nos_itoa(obj, buf, 10);
    return nos_print(out, buf);
}

int nos_print(nos::ostream &out, unsigned char obj)
{
    char buf[48];
    __nos_itoa(obj, buf, 10);
    return nos_print(out, buf);
}
int nos_print(nos::ostream &out, unsigned short obj)
{
    char buf[48];
    __nos_itoa(obj, buf, 10);
    return nos_print(out, buf);
}
int nos_print(nos::ostream &out, unsigned int obj)
{
    char buf[48];
    __nos_itoa(obj, buf, 10);
    return nos_print(out, buf);
}
int nos_print(nos::ostream &out, unsigned long obj)
{
    char buf[48];
    __nos_itoa(obj, buf, 10);
    return nos_print(out, buf);
}
int nos_print(nos::ostream &out, unsigned long long obj)
{
    char buf[48];
    __nos_itoa(obj, buf, 10);
    return nos_print(out, buf);
}

int nos_print(nos::ostream &out, float obj)
{
    char buf[48];
    __nos_ftoa(obj, buf, 5);
    return nos_print(out, buf);
}

int nos_print(nos::ostream &out, double obj)
{
    char buf[48];
    __nos_dtoa(obj, buf, 5);
    return nos_print(out, buf);
}

int nos_print(nos::ostream &out, long double obj)
{
    char buf[48];
    __nos_ldtoa(obj, buf, 5);
    return nos_print(out, buf);
}

int nos_print(nos::ostream &out, const nos::buffer &buf)
{
    return *out.write(buf.data(), buf.size());
}