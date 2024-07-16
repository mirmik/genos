#ifndef RALGO_HEIMER_AXISCTR_APPROVAL_H
#define RALGO_HEIMER_AXISCTR_APPROVAL_H

#include <igris/compiler.h>
#include <ralgo/heimer/heimer_types.h>

namespace heimer
{
    class axisctr_approval
    {
    public:
        virtual int check(int dim, position_t *strt, position_t *fini) = 0;
        virtual ~axisctr_approval() = default;
    };
}

#endif
