#ifndef HAL_ARCH_H_
# error "Do not include this file directly!"
#endif /* HAL_ARCH_H_ */

#include <compiler.h>
#include <asm/chip.h>

__BEGIN_DECLS

void arch_diag_init();
unsigned arch_external_generator_init(unsigned Mkoeff, unsigned Nkoeff, unsigned Pkoeff, unsigned Qkoeff, unsigned HSE);

__END_DECLS