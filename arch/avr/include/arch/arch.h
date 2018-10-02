#ifndef HAL_ARCH_H_
# error "Do not include this file directly!"
#endif /* HAL_ARCH_H_ */

#include <util/endian.h>

#ifndef __BYTE_ORDER
#define __BYTE_ORDER __LITTLE_ENDIAN
#endif

#define BYTE_ORDER    __BYTE_ORDER

#define PLATFORM_ARCH "avr"
