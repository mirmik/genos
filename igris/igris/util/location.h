#ifndef IGRIS_UTIL_LOCATION_H
#define IGRIS_UTIL_LOCATION_H

//Определение позиции в коде. Для трассировки.
#include <igris/compiler.h>

struct location
{
    int line;
    const char *file;
    const char *func;
};

#define CURRENT_LOCATION(name)                                                 \
    struct location name = {__LINE__, __FILE__, __func__};

#define CURRENT_LOCATION_INITARGS __LINE__, __FILE__, __func__

__BEGIN_DECLS

void debug_print_location(struct location location);

__END_DECLS

#endif
