#include <drivers/tty/tty_ldisc_classic.h>

void tty_ldisc_classic_newchar(
	struct tty_ldisc_classic * ldisc, char c) 
{

}

const struct tty_ldisc_operations = {
	.newchar = tty_ldisc_classic_newchar
};

void tty_ldisc_classic_init(
	struct tty_ldisc_classic * ldisc, 
	struct tty * tty,
	char * buffer,
	int    buflen) 
{
	tty_ldisc_init(ldisc, tty, tty_ldisc_classic_operations);
}