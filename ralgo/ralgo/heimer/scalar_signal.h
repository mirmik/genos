#ifndef RALGO_HEIMER_SCALAR_SIGNAL_H
#define RALGO_HEIMER_SCALAR_SIGNAL_H

#include <ralgo/heimer/signal.h>

namespace heimer
{
    class scalar_signal : public signal_head
    {
    public:
        double value = 0;

    public:
        scalar_signal(const char *name);
        int info(char *, int) override;
    };
}

#endif
