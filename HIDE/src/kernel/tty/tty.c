#include <communication/serial/tty.h>

#include <mem/alloc.h>
#include <assert.h>

int tty_init(struct tty_s* tty, struct tty_drv* driver, const struct tty_drv_ops* dops) {
	assert(driver);

	tty->drv 	 = driver;
	tty->dops = (struct tty_drv_ops*) dops;

	tty->ldisc = NULL;
	tty->lops = (struct tty_ldisc_ops*) &tty_straight_ldisc_operations;

	return 1;
}

int tty_set_ldisc(struct tty_s* tty, struct tty_ldisc *ldisc, const struct tty_ldisc_ops *lops) {
	assert(tty && lops);
	tty->lops = lops;
	tty->ldisc = ldisc;
} 

int tty_write(struct tty_s* tty, const void* data, size_t size) {
	assert (tty);
	return tty->lops->write(tty, data, size);
}

int tty_putchar(struct tty_s* tty, const char c) {
	assert (tty);
	return tty->lops->putchar(tty, c);
}