#include <genos/chardev.h>
#include <nos/io/generic_ostream.h>
#include <nos/print.h>
#include <serial.h>

zillot::uartring uring_rs232(&usart_rs232, 1024, 1024);
zillot::uartring uring_console(&usart_console, 1024, 1024);

genos::zillot_chardev rs232_chardev(&uring_rs232, "ttyS1");
genos::zillot_chardev console_chardev(&uring_console, "ttyS0");

nos::generic_ostream dbgout(+[](const void *data, size_t size) {
    uring_console.write((const char *)data, size);
    return 1;
});

void serial_interfaces_init()
{
    usart_rs232.enable(false);
    usart_rs232.setup(115200, 'n', 8, 1);
    usart_rs232.enable(true);

    usart_console.enable(false);
    usart_console.setup(115200, 'n', 8, 1);
    usart_console.enable(true);

    uring_rs232.begin();
    uring_console.begin();

    nos::set_default_ostream(&dbgout);
}