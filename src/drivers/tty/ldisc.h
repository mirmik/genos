#ifndef DRIVERS_TTY_DISCIPLINE_LDISC_H
#define DRIVERS_TTY_DISCIPLINE_LDISC_H

#include <drivers/tty/disc.h>

struct tty_ldisc;

struct tty_ldisc_operations 
{
	(void)(*newchar)(char c);
}

struct tty_ldisc 
{
	const struct tty_ldisc_operations * ops;
	struct tty * tty;
};

__BEGIN_DECLS

static inline
void tty_ldisc_init(
	struct tty_ldisc * ldisc,
	struct tty * tty,
	const struct tty_ldisc_operations * ops
) 
{
	ldisc->tty = tty;
	ldisc->ops = ops;
}

__END_DECLS

#endif