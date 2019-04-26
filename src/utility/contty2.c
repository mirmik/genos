#include <utility/contty2.h>

#include <drivers/cdev/cdev.h>
#include <igris/util/bug.h>
#include <igris/dprint.h>
#include <igris/defs/vt100.h>

#include <errno.h>
#include <utility/mshell.h>

static int newline(struct contty2_context * cntxt)
{
	int ret;
	ret = mshell_execn(cntxt->rl.line.buf, cntxt->rl.line.len, SH_INTERNAL_SPLIT, NULL);

	switch (ret)
	{
		case ENOENT: 
			cntxt->cdev->c_ops->write(
			    cntxt->cdev, "Entry not found\n\r", 17, 0);
			cntxt->cdev->c_ops->write(cntxt->cdev, "message: ", 9, 0);
			cntxt->cdev->c_ops->write(cntxt->cdev, cntxt->rl.line.buf, cntxt->rl.line.len, 0);
			cntxt->cdev->c_ops->write(cntxt->cdev, "\r\n", 2, 0);
			break;
	}

	return ret;

}

void contty2_debug_mode(struct contty2_context * cntxt, int en)
{
	cntxt->debug_mode = en;
}

void * contty2_automate(void * arg, int * state)
{
	struct file * filp;
	char c;
	int ret;

	struct contty2_context * cntxt = (struct contty2_context *) arg;
	struct char_device * cdev = cntxt->cdev;
	struct readline * rl = &cntxt->rl;

	switch (*state)
	{
		case 0:
			if (cntxt->debug_mode)
				dprln("contty: state 0");

			readline_init(rl, cntxt->buffer, CONTTY2_LINE_LENGTH);
			*state = 1;

		case 1:
			if (cntxt->debug_mode)
				dprln("contty: state 1");

			readline_newline_do(rl);
			cdev->c_ops->write(cdev, "$ ", 2, 0);
			*state = 2;

		case 2:
			if (cntxt->debug_mode)
				dprln("contty: state 2");

			cdev->c_ops->read(cdev, &c, 0, IO_ONLYWAIT); //Неблокирующий wait для автомата.
			*state = 3;
			break;

		case 3:
			if (cntxt->debug_mode)
				dprln("contty: state 3");

			while ((ret = cdev->c_ops->read(cdev, &c, 1, IO_NOBLOCK)))
			{
				if (cntxt->debug_mode)
				{
					dpr("contty: read char ");
					dprhex(c);
					dln();
					continue;
				}

				ret = readline_putchar(rl, c);

				switch (ret) 
				{
					case READLINE_ECHOCHAR:
						cdev->c_ops->write(cdev, &c, 1, 0);
						break;

					case READLINE_NEWLINE:
						*state = 1;
						cdev->c_ops->write(cdev, "\r\n", 2, 0);
						newline(cntxt);
						return NULL;

					case READLINE_BACKSPACE:
						cdev->c_ops->write(cdev, VT100_LEFT, 3, 0);
						cdev->c_ops->write(cdev, VT100_ERASE_LINE_AFTER_CURSOR, 3, 0);
						return NULL;

					case READLINE_RIGHT:
						cdev->c_ops->write(cdev, VT100_RIGHT, 3, 0);
						return NULL;

					case READLINE_LEFT:
						cdev->c_ops->write(cdev, VT100_LEFT, 3, 0);
						return NULL;

					case READLINE_NOTHING:
						break;

					default:
						dprln("retcode:", ret);
						BUG();
				}

			}
			*state = 2;
			break;

		default:
			BUG();
	}

	return NULL;
}