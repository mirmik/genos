#ifndef ZILLOT_UART_DEVICE_H_
#define ZILLOT_UART_DEVICE_H_

#include <igris/compiler.h>
#include <igris/util/stub.h>
#include <stdbool.h>
#include <stdint.h>

#define UART_IRQCODE_RX 0
#define UART_IRQCODE_TX 1 // Можно положить следующий байт
#define UART_IRQCODE_TC 2 // Конвеер завершил свою работу.

#define UART_IRQCODE_ERROR_MASK 0x80
#define UART_IRQCODE_RX_OVERRUN 0x81

#define UART_CTRIRQS_RXON 0
#define UART_CTRIRQS_RXOFF 1
#define UART_CTRIRQS_TXON 2
#define UART_CTRIRQS_TXOFF 3
#define UART_CTRIRQS_TCON 4
#define UART_CTRIRQS_TCOFF 5

namespace zillot
{
    typedef void (*uart_irq_handler_t)(void *, int);

    class uart
    {
    public:
        uart_irq_handler_t handler = nullptr;
        void *handarg = nullptr;

    public:
        virtual int setup(int32_t baud,
                          char parity,
                          uint8_t databits,
                          uint8_t stopbits) = 0;
        virtual int enable(int en) = 0;
        virtual int ctrirqs(uint8_t cmd) = 0;
        virtual int recvbyte() = 0;
        virtual int sendbyte(unsigned char c) = 0;
        virtual int cantx() = 0;
        virtual int hasrx() = 0;
        virtual ~uart() = default;
        void setup_irq_handler(uart_irq_handler_t handler, void *handarg)
        {
            this->handler = handler;
            this->handarg = handarg;
        }
    };
}

#endif
