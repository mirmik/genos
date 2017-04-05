#ifndef GENOS_KERNEL_TTY_H
#define GENOS_KERNEL_TTY_H

#include <compiler.h>
#include <stdlib.h>

struct tty_drv_ops;
struct tty_ldisc_ops;
struct tty_drv;
struct tty_ldisc;

struct tty_s {
	void *ldisc;
	void *drv;

	struct tty_ldisc_ops* lops;
	struct tty_drv_ops* dops;
};

/**/
struct tty_ldisc_ops {
	int (*putchar) (struct tty_s *tty, char c);
	int (*write) (struct tty_s *tty, const void *data, size_t size);
};

/**/
struct tty_drv_ops {
	int (*putchar) (struct tty_s *tty, char c);
	int (*write) (struct tty_s *tty, const void *data, size_t size);	
};

__BEGIN_DECLS

/*tty*/ 
int tty_init(struct tty_s* tty, struct tty_drv* drv, const struct tty_drv_ops* dops);
int tty_set_ldisc(struct tty_s* tty, struct tty_ldisc* ldisc, const struct tty_ldisc_ops* lops);
int tty_write(struct tty_s* tty, const void* data, size_t size);
int tty_putchar(struct tty_s* tty, const char c);

/*ldisc*/
extern struct tty_ldisc_ops tty_straight_ldisc_operations;

/*driver*/

__END_DECLS

#define TTY_STD_DEF(name,__drv,__dops) 								\
struct tty_s name = {												\
	.ldisc = NULL,													\
	.drv = __drv,													\
	.lops = &tty_straight_ldisc_operations,							\
	.dops = __dops,													\
};

#endif