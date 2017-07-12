#ifndef PRINTF_IMPL_H_
#define PRINTF_IMPL_H_

#include <stdarg.h>
#include <genos/compiler.h>

typedef void(*printchar_t)(void* d, char c);

__BEGIN_DECLS

int __print ( printchar_t handler, void* printchar_data,
			const char *format, va_list args );

__END_DECLS

#endif /* PRINTF_IMPL_H_ */
