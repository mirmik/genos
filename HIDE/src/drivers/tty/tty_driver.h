#ifndef GENOS_DRIVERS_TTY_DRIVER
#define GENOS_DRIVERS_TTY_DRIVER

struct tty_driver;

struct tty_driver_operations 
{
	int (*write)(struct tty_driver *, const char*, size_t);
}

struct tty_driver 
{
	const struct tty_driver_operations * ops;
	struct tty * tty; 
};

__BEGIN_DECL

static inline
void tty_driver_init(
	struct tty_driver * driver, 
	struct tty * tty,	
	const struct tty_driver_operations * ops
) 
{
	driver->tty = tty;
	driver->ops = ops;
}

static inline
void tty_driver_newchar(
	struct tty_driver * driver, char c) 
{
	driver->ldisc->ops->newchar(driver->ldisc, c);
}

static inline
void tty_driver_write(
	struct tty_driver * driver, 
	char * data,
	size_t size) 
{
	driver->ops->write(driver, data, size);
} 

__END_DECL

#endif