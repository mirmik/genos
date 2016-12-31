#include <communication/serial/tty.h>

int straight_ldisc_write(struct tty_s* tty, const void* data, size_t size) {
	tty->dops->write(tty, data, size);
}

int straight_ldisc_putchar(struct tty_s* tty, char c) {
	return tty->dops->putchar(tty, c);
}

struct tty_ldisc_ops tty_straight_ldisc_operations = {
	.write = straight_ldisc_write,
	.putchar = straight_ldisc_putchar,
};

