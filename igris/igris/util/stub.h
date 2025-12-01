#ifndef IGRIS_STUB_H
#define IGRIS_STUB_H

#include <igris/compiler.h>

__BEGIN_DECLS

// Функция не делающая ничего.
extern void do_nothing(void);

__END_DECLS

#ifdef __cplusplus

template <class R, class... Args>
constexpr R (*do_nothing_signature)(Args...) = (R(*)(Args...))do_nothing;

#endif

#endif
