#ifndef AVR_DEBUG_ASSEMBLER_H
#define AVR_DEBUG_ASSEMBLER_H

#ifdef __ASSEMBLER__

.global dasm_regmap_set
.global dasm_regmap_print
.global debug_regmap;

#else

#include <genos/compiler.h>
#include "inttypes.h"

__BEGIN_DECLS

struct __register_map
{
	uint8_t regs[32];
	uint8_t sreg;
	uint8_t spl;
	uint8_t sph;
	uint16_t ret;
};
extern struct __register_map debug_regmap;
void dasm_regmap_print();
void dasm_stack_dump(uint16_t len);

__END_DECLS

#endif

#endif