#include <ctype.h>
#include <igris/util/numconvert.h>
#include <stddef.h>
#include <stdlib.h>

#ifndef WITHOUT_ATOF64
__attribute__((weak)) double strtod(const char *nptr, char **endptr)
{
    return igris_atof64(nptr, endptr);
}

double atof(const char *nptr)
{
    return igris_atof64(nptr, NULL);
}
#else
__attribute__((weak)) double strtod(const char *nptr, char **endptr)
{
    return igris_atof32(nptr, endptr);
}

double atof(const char *nptr)
{
    return igris_atof32(nptr, NULL);
}
#endif
