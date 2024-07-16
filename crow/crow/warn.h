/** @file */

#ifndef CROW_WARN_H
#define CROW_WARN_H

#include <nos/buffer.h>

namespace crow
{
#if defined(MEMORY_ECONOMY)
    static inline void warn(nos::buffer) {}
#else
    void warn(nos::buffer msg);
#endif
}

#endif