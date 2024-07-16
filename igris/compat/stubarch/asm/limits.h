#ifndef IGRIS_STUBARCH_ASM_LIMITS_H
#define IGRIS_STUBARCH_ASM_LIMITS_H

#ifndef IGRIS_STUBARCH_WORDSIZE
#error "IGRIS_STUBARCH_WORDSIZE not defined"
#endif

#if IGRIS_STUBARCH_WORDSIZE == 16
#include <asm-generic/limits16.h>
#elif IGRIS_STUBARCH_WORDSIZE == 32
#include <asm-generic/limits32.h>
#elif IGRIS_STUBARCH_WORDSIZE == 64
#include <asm-generic/limits64.h>
#else
#error "Unsupported word size"
#endif

#endif