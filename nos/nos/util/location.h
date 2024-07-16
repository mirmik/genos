#ifndef NOS_UTIL_LOCATION_H
#define NOS_UTIL_LOCATION_H

struct nos_location
{
    int line;
    const char *file;
    const char *func;
};

#define NOS_CURRENT_LOCATION(name)                                             \
    struct nos_location name = {__LINE__, __FILE__, __func__};

#define NOS_CURRENT_LOCATION_INITARGS __LINE__, __FILE__, __func__

#endif