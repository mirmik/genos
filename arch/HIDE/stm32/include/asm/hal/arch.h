#ifndef HAL_ARCH_H_
# error "Do not include this file directly!"
#endif /* HAL_ARCH_H_ */

#ifndef __BYTE_ORDER__
#define __BYTE_ORDER __LITTLE_ENDIAN
#define BYTE_ORDER    __BYTE_ORDER
#endif

#include <compiler.h>

#include <periph/regs/usart.h>

__BEGIN_DECLS

void arch_diag_init();
void arch_external_generator_init();

__END_DECLS

#define PLATFORM_ARCH "stm32"

