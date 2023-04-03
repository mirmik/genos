#include <genos/drivers/zillot_tty_driver.h>

void genos::zillot_tty_driver::transmit(const char *data, size_t size) {}

void genos::zillot_tty_driver::uartring_irq_handler(void *priv, int code)
{
    switch (code)
    {
    case UART_IRQCODE_TX:
    {
        uartring_irq_handler_tx(priv);
        return;
    }

    case UART_IRQCODE_RX:
    {
        uartring_irq_handler_rx(priv);
        return;
    }

        // case UART_IRQCODE_TC: //fallthrow

    default:
        dprln("uring:", code);
        return;
    }
}

void genos::zillot_tty_driver::uartring_irq_handler_tx(void *priv)
{
    struct genos::zillot_tty_driver *uring =
        (struct genos::zillot_tty_driver *)priv;
    if (uring->txring.empty())
    {
        uring->udev->ctrirqs(UART_CTRIRQS_TXOFF);
        // uring->tx_callback.invoke();
        return;
    }
    char c = uring->txring.tail();
    uring->txring.pop();
    uring->udev->sendbyte(c);
}

void genos::zillot_tty_driver::uartring_irq_handler_rx(void *priv)
{
    struct genos::zillot_tty_driver *uring =
        (struct genos::zillot_tty_driver *)priv;
    char c = uring->udev->recvbyte();
    uring->receive_newchar(c);
}

void genos::zillot_tty_driver::begin()
{
    // udev = uart;
    // if (rxbuffer == nullptr)
    //    rxbuffer = (char *)malloc(rxring.size);
    // if (txbuffer == nullptr)
    //    txbuffer = (char *)malloc(txring.size);
    udev->handler = uartring_irq_handler;
    udev->handarg = (void *)this;
    udev->enable(1);
    udev->ctrirqs(UART_CTRIRQS_RXON);
}