#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include <igris/util/printf_impl.h>

struct printchar_handler_data
{
	int errcode;
	int fd;
};

static void file_printchar(void *d, int c)
{
	struct printchar_handler_data * data = (struct printchar_handler_data *) d;
	int ret = fdputc(c, data->fd);

	if (ret < 0 && data->errcode == 0 ) 
	{
		data->errcode = ret;
	}
}

int vfdprintf(int fd, const char *format, va_list args)
{
	struct printchar_handler_data data;

	data.fd = fd;
	data.errcode = 0;

	int ret = __printf(file_printchar, &data, format, args);
	return data.errcode != 0 ? data.errcode : ret;
}

int fdprintf(int fd, const char *format, ...)
{
	int ret;
	va_list args;

	va_start(args, format);
	ret = vfdprintf(fd, format, args);
	va_end(args);

	return ret;
}
