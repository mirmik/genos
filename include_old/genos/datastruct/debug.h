#ifndef DEBUG_DATASTRUCT_H
#define DEBUG_DATASTRUCT_H

#include "compiler.h"
#include "debug/dprint.h"
#include "genos/datastruct/cvector.h"
#include "genos/datastruct/ring_head.h"

__BEGIN_DECLS

static inline void dprint_cvector(struct cvector* cv)
{
	debug_print("cvector:\t"); debug_printhex_ptr(cv); dln();
	debug_print("cv->buffer:\t"); debug_printhex_ptr(cv->buffer); dln();
	debug_print("cv->capacity:\t"); debug_printdec_uint32(cv->capacity); dln();
	debug_print("cv->length:\t"); debug_printdec_uint32(cv->length); dln();
}

__END_DECLS

#endif