#include <utility/contty3.h>

#include <unistd.h>
#include <fcntl.h>

#include <drivers/cdev/serdev.h>
#include <igris/util/bug.h>
#include <igris/dprint.h>
#include <igris/defs/vt100.h>

#include <errno.h>
#include <utility/mshell.h>

static int newline(struct contty3_context * cntxt)
{
	/*int ret;
	cntxt->rl.line.buf[cntxt->rl.line.len] = 0;
	ret = mshell_execn(cntxt->rl.line.buf, cntxt->rl.line.len, SH_INTERNAL_SPLIT, NULL, 1);

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

	return ret;*/
	return 0;
}

void contty3_debug_mode(struct contty3_context * cntxt, int en)
{
	cntxt->debug_mode = en;
}

void * contty3_automate(void * arg, int * state)
{
	//struct file * filp;
	char c;
	int ret;

	struct contty3_context * cntxt = (struct contty3_context *) arg;
	
	cntxt->wfd = open(cntxt->iopath, 0);
	cntxt->rfd = open(cntxt->iopath, 0);
	
	//struct serial_device * cdev = cntxt->cdev;
	struct readline * rl = &cntxt->rl;

	switch (*state)
	{
		case 0:
			if (cntxt->debug_mode)
				dprln("contty: state 0");

			readline_init(rl, cntxt->buffer, CONTTY3_LINE_LENGTH);
			readline_history_init(rl, cntxt->hbuffer, CONTTY3_HISTORY_SIZE);
			*state = 1;

	/*	case 1:
			if (cntxt->debug_mode)
				dprln("contty: state 1");

			readline_newline_do(rl);
			*state = 2;
			cdev->c_ops->write(cdev, "$ ", 2, 0);

		case 2:
			if (cntxt->debug_mode)
				dprln("contty: state 2");

			*state = 3;
			cdev->c_ops->read(cdev, &c, 0, IO_ONLYWAIT); //Неблокирующий wait для автомата.
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
					{
						cdev->c_ops->write(cdev, &c, 1, 0);

						if (!sline_in_rightpos(&rl->line))
						{
							char buf[16];

							cdev->c_ops->write(cdev, sline_rightpart(&rl->line), sline_rightsize(&rl->line), 0);
							ret = vt100_left(buf, sline_rightsize(&rl->line));
							cdev->c_ops->write(cdev, buf, ret, 0);
						}
					}
					break;

					case READLINE_NEWLINE:
						*state = 1;
						cdev->c_ops->write(cdev, "\r\n", 2, 0);
						newline(cntxt);
						return NULL;

					case READLINE_BACKSPACE:
					{
						cdev->c_ops->write(cdev, VT100_LEFT, 3, 0);
						cdev->c_ops->write(cdev, VT100_ERASE_LINE_AFTER_CURSOR, 3, 0);

						if (!sline_in_rightpos(&rl->line))
						{
							char buf[16];

							cdev->c_ops->write(cdev, sline_rightpart(&rl->line), sline_rightsize(&rl->line), 0);
							ret = vt100_left(buf, sline_rightsize(&rl->line));
							cdev->c_ops->write(cdev, buf, ret, 0);
						}
						break;
					}
					

					case READLINE_RIGHT:
						cdev->c_ops->write(cdev, VT100_RIGHT, 3, 0);
						break;

					case READLINE_LEFT:
						cdev->c_ops->write(cdev, VT100_LEFT, 3, 0);
						break;

					case READLINE_NOTHING:
						break;

					case READLINE_UPDATELINE:
					{
						char buf[16];

						if (rl->lastsize)
						{
							ret = vt100_left(buf, rl->lastsize);
							cdev->c_ops->write(cdev, buf, ret, 0);
							cdev->c_ops->write(cdev, VT100_ERASE_LINE_AFTER_CURSOR, 3, 0);
						}
						if (rl->line.len)
							cdev->c_ops->write(cdev, rl->line.buf, rl->line.len, 0);
						break;
					}

					default:
						dprln("retcode:", ret);
						BUG();
				}

			}
			*state = 2;
			break;*/

		default:
			BUG();
	}

	return NULL;
}