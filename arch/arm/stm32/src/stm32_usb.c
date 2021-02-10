#include <asm/stm32_usb.h>
#include <periph/map.h>

void TO_WRITE_PMA(unsigned char *P1, int P2, int N)
{
    unsigned short int *P1_1;
    unsigned int *P2_1;
    int i, buf1;;

    P1_1 = P1;
    P2_1 = P2 * 2;
    P2_1 = P2_1 + 0x10001800; // Это 0x40006000 / 4
    for (i = 0; i < N; i++)
    {
        buf1 = *P1_1;
        *P2_1 = buf1;
        P1_1++;
        P2_1++;
    }

}


void stm32_usb_fs_reset(USB_TypeDef * usb)
{
    int buf1;

    RCC -> APB1ENR1 |= RCC_APB1ENR1_USBFSEN;    // Без включения тактирования, пакетная память не работает (возвращаются только нули)

    usb->CNTR = 0;
    usb->ISTR = 0;
    usb->BTABLE = 0;

    buf1 = usb->EP0R;
    buf1 = buf1 ^ USB_EPRX_STAT; // Включить два бита STAT_RX с учетом того, что они имеют тип toggle
    buf1 = buf1 & USB_EPRX_STAT; // Сбросить все биты кроме STAT_RX
    buf1 = buf1 | (1 << 9);   // USB_EP_TYPE 

    usb->EP0R = buf1;
    usb->DADDR = 128;
    usb->ISTR &= ~USB_ISTR_RESET;



    /*    int buf1;

        unsigned short int DISCRIPTOR0[4] = {   64,             // ADDR_TX = 64 оставлено место для всех 8 дискрипторов перед буфером приема(можно оставить меньше)
                                                0,              // COUNT_TX = 0 нужно будет заполнить перед отправкой данных
                                                128,            // ADDR_RX = 128 пропускаем 64 байта буфера передачи
                                                33792
                                            };         // Указываем размер буфера приема 64 байта


        RCC -> APB1ENR1 |= RCC_APB1ENR1_USBEN;    // Без включения тактирования, пакетная память не работает (возвращаются только нули)

        TO_WRITE_PMA(DISCRIPTOR0, 0, 4);        // Заполняем дискриптор нулевой точки

        CNTR = 0;
        BTABLE = 0;                             // Адрес таблицы дискрипторов внутри пакетной памяти
        ISTR = 0;                               // Флаги прерываний(событий) USB

        // Инициализация EP0R. Сложность в том, что разные биты имеют разный тип обращения (RW, toggle, и. др.)
        // Разрешен прием данных, установлен тип точки CONTROL, запрещена передача
        buf1 = EP0R;
        buf1 = buf1 ^ 12288;                    //0011000000000000b ( ^ - это XOR ) (Включить два бита STAT_RX с учетом того, что они имеют тип toggle)
        buf1 = buf1 & 12288;                    //0011000000000000b (Сбросить все биты кроме STAT_RX)
        buf1 = buf1 | 512;                      //0000001000000000b (Один бит нужно просто установить (EP_TYPE) (он просто типа RW)
        EP0R = buf1;

        DADDR = 128;                            // Включаем модуль USB, адрес устройства 0

        ISTR &= ~ISTR_RESET;*/
}
