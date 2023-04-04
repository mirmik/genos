#ifndef GENOS_DRIVERS_ZILLOT_TTY_DRIVER_H
#define GENOS_DRIVERS_ZILLOT_TTY_DRIVER_H

#include <genos/drivers/tty_driver.h>
#include <igris/container/ring.h>
#include <zillot/common/uart.h>

namespace genos
{
    class zillot_tty_driver : public tty_driver
    {
        zillot::uart *udev = nullptr;
        igris::ring<char> txring = {};

    public:
        zillot_tty_driver() = default;

        zillot_tty_driver(zillot::uart *uart, size_t sz)
        {
            init(uart, sz);
        }

        void init(zillot::uart *uart, size_t sz)
        {
            this->udev = uart;
            txring.resize(sz);
        }

        void
        setup(int32_t baud, char parity, uint8_t databits, uint8_t stopbits);

        static void uartring_irq_handler_tx(void *priv);
        static void uartring_irq_handler_rx(void *priv);
        static void uartring_irq_handler(void *priv, int code);

        size_t transmit(const char *data, size_t size) override;

        virtual ~zillot_tty_driver() = default;

        void begin();
    };
}

#endif