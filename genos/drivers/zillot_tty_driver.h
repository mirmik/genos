#ifndef GENOS_DRIVERS_ZILLOT_TTY_DRIVER_H
#define GENOS_DRIVERS_ZILLOT_TTY_DRIVER_H

#include <genos/drivers/tty_driver.h>
#include <zillot/common/uart.h>

namespace genos
{
    class zillot_tty_driver : public tty_driver
    {
        zillot::uart *uart;

    public:
        zillot_tty_driver(zillot::uart *uart) : uart(uart) {}

        void transmit(const char *data, size_t size) override;
    };
}

#endif