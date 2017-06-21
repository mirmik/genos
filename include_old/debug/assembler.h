#ifndef GENOS_DEBUG_ASSEMBLER_LINKAGE_H
#define GENOS_DEBUG_ASSEMBLER_LINKAGE_H

#include "asm/debug/assembler.h"

#ifdef __ASSEMBLER__

.global dasm_test
.global dasm_stack_dump
.global debug_print_dump

.global debug_printhex_uint8
.global debug_printhex_uint16
.global debug_printhex_uint32
.global debug_printhex_uint64

.global debug_print_line

.global arch_halt

#endif

#endif