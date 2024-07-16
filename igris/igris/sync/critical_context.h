#ifndef IGRIS_SYNC_IS_CRITICAL_CONTEXT_H
#define IGRIS_SYNC_IS_CRITICAL_CONTEXT_H

#include <igris/compiler.h>

__BEGIN_DECLS

void critical_context_level_inc();
void critical_context_level_dec();
int critical_context_level();

__END_DECLS

#endif
