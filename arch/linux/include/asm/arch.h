#ifndef HAL_ARCH_H_
# error "Do not include this file directly!"
#endif /* HAL_ARCH_H_ */

#include "endian.h"

#ifndef __BYTE_ORDER__
#define __BYTE_ORDER __LITTLE_ENDIAN
#define BYTE_ORDER    __BYTE_ORDER
#endif

#define PLATFORM_ARCH "linux64"
