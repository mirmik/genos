#include <asm/avr_usart.h>
#include <zillot/avr/usart.h>
#include <igris/util/cpu_delay.h>
#include <igris/util/member.h>
#include <zillot/serial/avr_usart_device.h>

#include <stdbool.h>

int zillot::avr::usart::enable(int en)
{
    avr_usart_enable_rx(regs, en);
    avr_usart_enable_tx(regs, en);
    avr_usart_tcirq(regs, 0);
    avr_usart_txirq(regs, 0);
    avr_usart_rxirq(regs, en);

    // TODO: wait ready
    cpu_delay(10000);

    return 0;
}

int zillot::avr::usart::ctrirqs(uint8_t cmd)
{
    switch (cmd)
    {
    case UART_CTRIRQS_TXON:
        avr_usart_txirq(regs, true);
        return 0;

    case UART_CTRIRQS_TXOFF:
        avr_usart_txirq(regs, false);
        return 0;

    case UART_CTRIRQS_RXON:
        avr_usart_rxirq(regs, true);
        return 0;

    case UART_CTRIRQS_RXOFF:
        avr_usart_rxirq(regs, false);
        return 0;

    case UART_CTRIRQS_TCON:
        avr_usart_tcirq(regs, true);
        return 0;

    case UART_CTRIRQS_TCOFF:
        avr_usart_tcirq(regs, false);
        return 0;
    }

    return -1;
}

int zillot::avr::usart::recvbyte()
{
    return avr_usart_recvbyte(regs);
}

int zillot::avr::usart::sendbyte(unsigned char symbol)
{
    return avr_usart_sendbyte(regs, symbol);
}

int zillot::avr::usart::cantx()
{
    return avr_usart_cansend(regs);
}

int zillot::avr::usart::hasrx()
{
    return avr_usart_canrecv(regs);
}

/*void zillot::avr::usart::rx_irq_handler(void *arg)
{
    if (usart->dev.handler)
        usart->dev.handler(usart->dev.handarg, UART_IRQCODE_RX);
}

void zillot::avr::usart::dre_irq_handler(void *arg)
{
    if (usart->dev.handler)
        usart->dev.handler(usart->dev.handarg, UART_IRQCODE_TX);
}

///Прерывания опустошения конвеера. Обычно не требуется.
void zillot::avr::usart::tx_irq_handler(void *arg)
{
    if (usart->dev.handler)
        usart->dev.handler(usart->dev.handarg, UART_IRQCODE_TC);
}*/

/*#if defined DRIVERS_WITH_IRQTABLE
int zillot::avr::usart::irqinit()
{
    zillot::avr::usart::s * dev = mcast_out(priv, zillot::avr::usart::s, dev);

    irqtable_set_handler(base_irq    , _rx_handler, this);
    irqtable_set_handler(base_irq + 1, _dre_handler, this);
    irqtable_set_handler(base_irq + 2, _tx_handler, this);

    return 0;
}
#endif*/

int zillot::avr::usart::setup(int32_t baud, char parity,
                           uint8_t databits, uint8_t stopbits)
{
    avr_usart_enable_tx(regs, 1);
    avr_usart_enable_rx(regs, 1);
    avr_usart_setup(regs, baud, parity, databits, stopbits);
    return 0;
}