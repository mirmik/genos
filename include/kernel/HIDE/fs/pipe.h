#ifndef GENOS_FS_PIPE_H
#define GENOS_FS_PIPE_H

#include <kernel/fs/fs.h>
#include <genos/datastruct/ring_head.h>

struct pipe_s;

struct pipe_s {
	struct ring_head ring;
	char* buffer;
};

int 	pipe_open	(struct file *filep);
int     pipe_close	(struct file *filep);
ssize_t pipe_read	(struct file *filep, char *buffer, size_t buflen);
ssize_t pipe_write	(struct file *filep, const char *buffer, size_t buflen);
off_t   pipe_seek	(struct file *filep, off_t offset, int whence);
int     pipe_ioctl	(struct file *filep, int cmd, unsigned long arg);

static const struct file_operations pipe_file_operations = {
	pipe_open, 	/*open*/
	pipe_close, /*close*/
	pipe_read, 	/*read*/
	pipe_write, /*write*/
	pipe_seek, 	/*seek*/
	pipe_ioctl, /*ioctl*/
};

#endif