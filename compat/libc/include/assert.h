#ifndef ASSERT_H_
#define ASSERT_H_

#include <igris/dprint/dprint.h>
#include <stdlib.h>

#ifndef NDEBUG
static inline void __assert(const char *message, const char *file, int line)
{
    dprln("assert: ");
    DPRINT(message);
    DPRINT(file);
    DPRINT(line);
    abort();
    while (1)
        ;
}
#define assert(EX)                                                             \
    if (!(EX))                                                                 \
    __assert(#EX, __FILE__, __LINE__)
#else
#define assert(EX)
#endif

#ifndef __cplusplus
#define static_assert(a, b) _Static_assert(a, b)
#endif

#endif /* ASSERT_H_ */
