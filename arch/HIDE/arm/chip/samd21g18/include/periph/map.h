#ifndef GENOS_SAMD21G18_H
#define GENOS_SAMD21G18_H

#include <periph/regs/port.h>
#include <periph/regs/sercom.h>

#define GPIOA ((struct port_group_regs *)0x41004400UL)

#define SERCOM0 ((struct sercom_regs*)0x42000800UL)
#define SERCOM1 ((struct sercom_regs*)0x42000C00UL)
#define SERCOM2 ((struct sercom_regs*)0x42001000UL)
#define SERCOM3 ((struct sercom_regs*)0x42001400UL)
#define SERCOM4 ((struct sercom_regs*)0x42001800UL)
#define SERCOM5 ((struct sercom_regs*)0x42001C00UL)

#endif