/**
    @file
*/

#ifndef NOS_BUGON_H
#define NOS_BUGON_H

#include <igris/compiler.h>
#include <igris/dprint.h>
#include <igris/util/location.h>

__BEGIN_DECLS

__NORETURN void bug_impl();

__END_DECLS

#define BUG()                                                                  \
    {                                                                          \
        CURRENT_LOCATION(loc);                                                 \
        debug_print_location(loc);                                             \
        bug_impl();                                                            \
    }

#define BUG_ON(eq)                                                             \
    {                                                                          \
        if (eq)                                                                \
        {                                                                      \
            CURRENT_LOCATION(loc);                                             \
            debug_print_location(loc);                                         \
            dprln(#eq);                                                        \
            bug_impl();                                                        \
        }                                                                      \
    }

#endif
