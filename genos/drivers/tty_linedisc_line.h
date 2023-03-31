#ifndef GENOS_DRIVERS_TTY_LINEDISC_LINE_H
#define GENOS_DRIVERS_TTY_LINEDISC_LINE_H

#include <genos/drivers/tty.h>
#include <genos/drivers/tty_linedisc.h>

namespace genos
{
    class tty_linedisc_line : public genos::tty_linedisc
    {
        ring_head rxring;
        char buffer[48];

    public:
    };
}

#endif