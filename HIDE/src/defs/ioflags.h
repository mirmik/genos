#ifndef MVFS_IOFLAGS_H
#define MVFS_IOFLAGS_H

#define IO_NOBLOCK 0x01 // Отпускать управление, даже если записаны не все данные или данных на чтение нет 
#define IO_HOTLOOP 0x02 // Ждать, пока все данные будут записаны в буффер в горячем цикле.
#define IO_ONLYWAIT 0x04 // Не читать данные, а только ждать их появления
#define IO_WAITSEND 0x08 // Ждать в горячем цикле, пока все данные будут отправлены.

#endif