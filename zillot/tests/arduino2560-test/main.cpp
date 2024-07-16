#include <Arduino.h>
#include <asm/avr_arch.h>
#include <asm/avr_gpio.h>
#include <asm/avr_usart.h>
#include <zillot/avr/usart.h>
#include <zillot/common/uartring.h>
#include <igris/dprint.h>
#include <igris/time/jiffies-systime.h>
#include <igris/util/cpu_delay.h>
#include <new> 

AVR_USART_WITH_IRQS(usart0, USART0, USART0);
ZILLOT_UARTRING(serial0, usart0, 128, 16);

int main()
{
    arch_init();

    irqs_enable();
    usart0.setup(115200, 'n', 8, 1);
    serial0.begin();
    pinMode(13, 1);

    while (1)
    {
        //dprln(igris::millis());
        digitalWrite(13, 1);
        delay(500);
        usart0.sendbyte('A');
        digitalWrite(13, 0);
        delay(500);
        serial0.println("HelloWorld");
    }
}