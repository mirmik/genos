#ifndef ZILLOT_SERIAL_UARTRING_H
#define ZILLOT_SERIAL_UARTRING_H

#include <igris/buffer.h>
#include <igris/datastruct/dlist.h>
#include <igris/datastruct/ring.h>
#include <zillot/common/chardev.h>
#include <zillot/common/uart.h>

namespace zillot
{
    class uartring : public zillot::chardev
    {
        zillot::uart *udev = nullptr;
        char *rxbuffer = nullptr;
        char *txbuffer = nullptr;
        struct ring_head rxring;
        struct ring_head txring;
        int refs = 0;

    public:
        uartring(zillot::uart *udev, igris::buffer rxbuf, igris::buffer txbuf)
            : zillot::chardev(), udev(udev), rxbuffer(rxbuf.data()),
              txbuffer(txbuf.data()),
              rxring(RING_HEAD_INIT((unsigned int)rxbuf.size())),
              txring(RING_HEAD_INIT((unsigned int)txbuf.size()))
        {
        }

        uartring(zillot::uart *udev, size_t rxlen, size_t txlen)
            : zillot::chardev(), udev(udev),
              rxring(RING_HEAD_INIT((unsigned int)rxlen)),
              txring(RING_HEAD_INIT((unsigned int)txlen))
        {
        }

        uartring(const uartring &) = delete;
        uartring &operator=(const uartring &) = delete;

        int read(void *data, size_t size) override;
        int write(const void *data, size_t size) override;
        int release() override;
        int open(void *controller_id) override;

        size_t room() override;
        size_t avail() override;

        void begin(zillot::uart *udev);
        void begin()
        {
            begin(udev);
        };
        void clear();

        char getc()
        {
            char c;
            read(&c, 1);
            return c;
        }

        void putc(char c)
        {
            write(&c, 1);
        }

        static void uartring_irq_handler_tx(void *priv);
        static void uartring_irq_handler_rx(void *priv);
        static void uartring_irq_handler(void *priv, int code);
    };
}

#define ZILLOT_UARTRING(name, uart, tsize, rsize)                              \
    char CONCAT2(txbuf, __LINE__)[tsize];                                      \
    char CONCAT2(rxbuf, __LINE__)[rsize];                                      \
    zillot::uartring name(&(uart),                                             \
                          igris::buffer{CONCAT2(txbuf, __LINE__), tsize},      \
                          igris::buffer{CONCAT2(rxbuf, __LINE__), rsize});

#endif