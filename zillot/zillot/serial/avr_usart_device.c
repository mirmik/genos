#include <asm/avr_usart.h>
#include <igris/util/cpu_delay.h>
#include <igris/util/member.h>
#include <zillot/serial/avr_usart_device.h>

#include <stdbool.h>

int avr_usart_device_enable(uart_s *priv, int en)
{
    avr_usart_device_s *dev = mcast_out(priv, avr_usart_device_s, dev);

    avr_usart_enable_rx(dev->regs, en);
    avr_usart_enable_tx(dev->regs, en);
    avr_usart_tcirq(dev->regs, 0);
    avr_usart_txirq(dev->regs, 0);
    avr_usart_rxirq(dev->regs, en);

    // TODO: wait ready
    cpu_delay(10000);

    return 0;
}

int avr_usart_device_ctrirqs(uart_s *priv, uint8_t cmd)
{
    avr_usart_device_s *dev = mcast_out(priv, avr_usart_device_s, dev);

    switch (cmd)
    {
    case UART_CTRIRQS_TXON:
        avr_usart_txirq(dev->regs, true);
        return 0;

    case UART_CTRIRQS_TXOFF:
        avr_usart_txirq(dev->regs, false);
        return 0;

    case UART_CTRIRQS_RXON:
        avr_usart_rxirq(dev->regs, true);
        return 0;

    case UART_CTRIRQS_RXOFF:
        avr_usart_rxirq(dev->regs, false);
        return 0;

    case UART_CTRIRQS_TCON:
        avr_usart_tcirq(dev->regs, true);
        return 0;

    case UART_CTRIRQS_TCOFF:
        avr_usart_tcirq(dev->regs, false);
        return 0;
    }

    return -1;
}

int avr_usart_device_recvbyte(uart_s *priv)
{
    avr_usart_device_s *dev = mcast_out(priv, avr_usart_device_s, dev);
    return avr_usart_recvbyte(dev->regs);
}

int avr_usart_device_sendbyte(uart_s *priv, int symbol)
{
    avr_usart_device_s *dev = mcast_out(priv, avr_usart_device_s, dev);
    return avr_usart_sendbyte(dev->regs, symbol);
}

int avr_usart_device_cantx(uart_s *priv)
{
    avr_usart_device_s *dev = mcast_out(priv, avr_usart_device_s, dev);
    return avr_usart_cansend(dev->regs);
}

int avr_usart_device_hasrx(uart_s *priv)
{
    avr_usart_device_s *dev = mcast_out(priv, avr_usart_device_s, dev);
    return avr_usart_canrecv(dev->regs);
}

void avr_usart_device_rx_irq_handler(void *arg)
{
    struct avr_usart_device_s *usart = (struct avr_usart_device_s *)arg;
    if (usart->dev.handler)
        usart->dev.handler(usart->dev.handarg, UART_IRQCODE_RX);
}

void avr_usart_device_dre_irq_handler(void *arg)
{
    struct avr_usart_device_s *usart = (struct avr_usart_device_s *)arg;
    if (usart->dev.handler)
        usart->dev.handler(usart->dev.handarg, UART_IRQCODE_TX);
}

///Прерывания опустошения конвеера. Обычно не требуется.
void avr_usart_device_tx_irq_handler(void *arg)
{
    struct avr_usart_device_s *usart = (struct avr_usart_device_s *)arg;
    if (usart->dev.handler)
        usart->dev.handler(usart->dev.handarg, UART_IRQCODE_TC);
}

/*#if defined DRIVERS_WITH_IRQTABLE
int avr_usart_device_irqinit()
{
    avr_usart_device_s * dev = mcast_out(priv, avr_usart_device_s, dev);

    irqtable_set_handler(base_irq    , _rx_handler, this);
    irqtable_set_handler(base_irq + 1, _dre_handler, this);
    irqtable_set_handler(base_irq + 2, _tx_handler, this);

    return 0;
}
#endif*/

int avr_usart_device_setup(uart_s *priv, int32_t baud, char parity,
                           uint8_t databits, uint8_t stopbits)
{
    avr_usart_device_s *dev = mcast_out(priv, avr_usart_device_s, dev);

    avr_usart_enable_tx(dev->regs, 1);
    avr_usart_enable_rx(dev->regs, 1);

    avr_usart_setup(dev->regs, baud, parity, databits, stopbits);
    avr_usart_rxirq(dev->regs, 1);

    return 0;
}

const struct uart_operations avr_usart_device_ops = {
    .setup = avr_usart_device_setup,
    .enable = avr_usart_device_enable,
    .ctrirqs = avr_usart_device_ctrirqs,
    .recvbyte = avr_usart_device_recvbyte,
    .sendbyte = avr_usart_device_sendbyte,
    .cantx = avr_usart_device_cantx,
    .hasrx = avr_usart_device_hasrx,
};
