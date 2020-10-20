#ifndef GENOS_DRIVERS_CLASSIC_LDISC_H
#define GENOS_DRIVERS_CLASSIC_LDISC_H

struct tty_ldisc_classic 
{
	struct tty_ldisc ldisc;
	struct sline sline;
};

__BEGIN_DECLS

void tty_ldisc_classic_init(struct tty_ldisc * ldisc);

__END_DECLS

#endif