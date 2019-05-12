#ifndef GENOS_DRIVERS_TTY_H
#define GENOS_DRIVERS_TTY_H

#include <drivers/cdev/cdev.h>
#include <mvfs/node.h>

#include <sched/schedee.h>

struct tty;
struct tty_driver;
struct tty_ldisc;

struct tty_ldisc_operations 
{
	int (*write)(struct tty_ldisc * ldisc, char* data, int size) //< препроцессинг выходных данных
	int (*recv_handler)(struct tty_ldisc * ldisc, char c); //< препроцессинг входных данных
};

struct tty_driver_operations 
{
	int (*write)(struct tty_driver * drv, char* data, int size);
};

extern node_operations tty_node_ops;

struct tty_ldisc
{
	const struct tty_ldisc_operations* ops;
	struct tty * tty;
};

struct tty_driver
{
	const struct tty_driver_operations* ops;
	struct tty * tty;
};

struct tty
{
	struct node node;

	struct tty_ldisc *ldisc;
	struct tty_driver *drv;

	struct schedee * opensch;
};