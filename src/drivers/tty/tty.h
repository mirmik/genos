#ifndef GENOS_DRIVERS_TTY_H
#define GENOS_DRIVERS_TTY_H

#include <fs/file.h>
#include <drivers/char/char_device.h>

struct tty_struct;

struct tty_driver_operations {

};

struct tty_ldisc_operations {
	void (*write)(struct tty_struct* tty, const char* data, unsigned int size);
	void (*read)(struct tty_struct* tty, char* data, unsigned int size);

	void (*sendall_handler)(void* ldisc); ///< driver's callback for transvered bytes.
	void (*recvbyte_handler)(void* ldisc, int byte); ///< driver's callback for received bytes.  
};

struct tty_struct {
	struct char_device cdev;

	const struct tty_driver_operations* driver_ops;
	void* driver_data;

	const struct tty_ldisc_operations* ldisc_ops;
	void* ldisc_data;	
};


static inline struct tty_struct *file_tty(struct file *file) {
	return (struct tty_struct*) file->privdata;
}

int tty_open(struct inode *inode, struct file *filp); //struct inode *inode, struct file *filp
int tty_release(struct inode *inode, struct file *filp);
int tty_write(struct file *file, const char *data, unsigned int size);
int tty_read(struct file *file, char *data, unsigned int size);

static const struct file_operations tty_fops = {
	.open		= tty_open,
	.release	= tty_release,
//	.llseek		= no_llseek,
	.read		= tty_read,
	.write		= tty_write,
//	.poll		= tty_poll,
//	.unlocked_ioctl	= tty_ioctl,
//	.compat_ioctl	= tty_compat_ioctl,
//	.fasync		= tty_fasync,
};

#endif