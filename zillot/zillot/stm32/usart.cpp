#include <chip/irqdefs.h>
#include <zillot/armv7e-m/arm_nvic.h>
#include <zillot/irqtable/irqtable.h>
#include <zillot/stm32/pin.h>
#include <zillot/stm32/stm32_gpio.h>
#include <zillot/stm32/stm32_usart.h>
#include <zillot/stm32/usart.h>

void zillot::stm32::usart::init_gpio(zillot::stm32::pin tx,
                                     zillot::stm32::pin rx,
                                     int af)
{
    stm32_rcc_enable_gpio(tx.regs);
    stm32_rcc_enable_gpio(rx.regs);

    stm32_gpio_setup(tx.regs, tx.mask, GPIO_MODE_ALTERNATE);
    stm32_gpio_setup(rx.regs, rx.mask, GPIO_MODE_ALTERNATE);

    stm32_gpio_set_alternate(tx.regs, tx.mask, af);
    stm32_gpio_set_alternate(rx.regs, rx.mask, af);
}

int zillot::stm32::usart::enable(int en)
{
    stm32_usart_enable(regs, en);
    return 0;
}

int zillot::stm32::usart::ctrirqs(uint8_t cmd)
{
    switch (cmd)
    {
    case UART_CTRIRQS_TXON:
        stm32_usart_txirq_enable(regs, true);
        return 0;

    case UART_CTRIRQS_TXOFF:
        stm32_usart_txirq_enable(regs, false);
        return 0;

    case UART_CTRIRQS_RXON:
        stm32_usart_rxirq_enable(regs, true);
        return 0;

    case UART_CTRIRQS_RXOFF:
        stm32_usart_rxirq_enable(regs, false);
        return 0;

    case UART_CTRIRQS_TCON:
        stm32_usart_tcirq_enable(regs, true);
        return 0;

    case UART_CTRIRQS_TCOFF:
        stm32_usart_tcirq_enable(regs, false);
        return 0;
    }

    return -1;
}

int zillot::stm32::usart::recvbyte()
{
    return stm32_usart_getc(regs);
}

int zillot::stm32::usart::sendbyte(unsigned char symbol)
{
    return stm32_usart_putc(regs, symbol);
}

int zillot::stm32::usart::cantx()
{
    return stm32_usart_room(regs);
}

int zillot::stm32::usart::hasrx()
{
    return stm32_usart_avail(regs);
}

static void _irqhandler(void *priv)
{
    zillot::stm32::usart *dev = (zillot::stm32::usart *)priv;
    USART_TypeDef *regs = dev->regs;

    if (stm32_rxirq_status(regs) && stm32_usart_rxirq_enabled(regs))
    {
        dev->handler(dev->handarg, UART_IRQCODE_RX);
    }

    else if (stm32_txirq_status(regs) && stm32_usart_txirq_enabled(regs))
    {
        dev->handler(dev->handarg, UART_IRQCODE_TX);
    }

    else if (stm32_tcirq_status(regs) && stm32_usart_tcirq_enabled(regs))
    {
        dev->handler(dev->handarg, UART_IRQCODE_TC);
    }

    else if (stm32_overrun_irq_status(regs))
    {
        stm32_drop_overrun_flag(regs);
        dev->handler(dev->handarg, UART_IRQCODE_RX_OVERRUN);
    }

    else
    {
        // dpr("stm32: unh usart irq usart->ISR: ");
        // stm32_debug_print_usart_interrupt_status_register(dev->regs);
        // dln();
        abort();
    }
}

#ifdef ZILLOT_WITH_IRQTABLE
void zillot::stm32::usart::irqinit()
{
    irqtable_set_handler(irqno, &_irqhandler, (void *)this);
    nvic_enable_irq(irqno);
}
#endif

int zillot::stm32::usart::setup(int32_t baud,
                                char parity,
                                uint8_t databits,
                                uint8_t stopbits)
{
    // STM32 count parity bit as one of databit.
    if (parity != 'n')
        databits += 1;

    stm32_usart_setup(regs, baud, parity, databits, stopbits);

#ifdef ZILLOT_WITH_IRQTABLE
    irqinit();
#endif

    return 0;
}