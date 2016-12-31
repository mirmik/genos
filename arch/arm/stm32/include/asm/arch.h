#ifndef HAL_ARCH_H_
# error "Do not include this file directly!"
#endif /* HAL_ARCH_H_ */

#include <compiler.h>
#include <asm/chip.h>

__BEGIN_DECLS

void arch_diag_init();
void arch_external_generator_init();

__END_DECLS