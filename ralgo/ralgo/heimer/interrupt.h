#ifndef RALGO_HEIMER_INTERRUPT_H
#define RALGO_HEIMER_INTERRUPT_H

#include <igris/compiler.h>

__BEGIN_DECLS

void throw_interrupt(struct signal_processor *source, char *message);

__END_DECLS

#endif
