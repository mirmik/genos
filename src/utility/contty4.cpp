#include <utility/contty2.h>

#include <drivers/cdev/serdev.h>
#include <igris/util/bug.h>
#include <igris/dprint.h>
#include <igris/defs/vt100.h>

#include <igris/shell/readline.h>
#include <conf.h>

#include <errno.h>

static int newline(struct contty2_context * cntxt)
{
	char data[48];
	size_t sz = readline_linecpy(&cntxt->rl, data, 48);
	data[sz] = '\n';
	//data[sz+1] = '\r';
	//data[sz+2] = 0;
	dprdump(data, sz+1);
	SCPI_Input(&scpi_context, data, sz+1);
}

void contty2_debug_mode(struct contty2_context * cntxt, int en)
{
	cntxt->debug_mode = en;
}

void * contty2_automate(void * arg, int * state)
{
	//struct file * filp;
	char c;
	int ret;

	struct contty2_context * cntxt = (struct contty2_context *) arg;
	genos::char_device * cdev = cntxt->cdev;
	struct readline * rl = &cntxt->rl;

	switch (*state)
	{
		case 0:
			if (cntxt->debug_mode)
				dprln("contty: state 0");

			readline_init(rl, cntxt->buffer, CONTTY2_LINE_LENGTH);
			readline_history_init(rl, cntxt->hbuffer, CONTTY2_HISTORY_SIZE);
			*state = 1;

		case 1:
			if (cntxt->debug_mode)
				dprln("contty: state 1");

			readline_newline_reset(rl);
			*state = 2;
			//dev->write("$ ", 2, 0);

		case 2:
			if (cntxt->debug_mode)
				dprln("contty: state 2");

			*state = 3;
			cdev->read(&c, 0, IO_ONLYWAIT); //Неблокирующий wait для автомата.
			break;

		case 3:
			if (cntxt->debug_mode)
				dprln("contty: state 3");

			while ((ret = cdev->read(&c, 1, IO_NOBLOCK)))
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
						cdev->write(&c, 1, 0);

						if (!sline_in_rightpos(&rl->line))
						{
							char buf[16];

							cdev->write(sline_rightpart(&rl->line), sline_rightsize(&rl->line), 0);
							ret = vt100_left(buf, sline_rightsize(&rl->line));
							cdev->write(buf, ret, 0);
						}
					}
					break;

					case READLINE_NEWLINE:
						*state = 1;
						cdev->write("\r\n", 2, 0);
						newline(cntxt);
						return NULL;

					case READLINE_BACKSPACE:
					{
						cdev->write(VT100_LEFT, 3, 0);
						cdev->write(VT100_ERASE_LINE_AFTER_CURSOR, 3, 0);

						if (!sline_in_rightpos(&rl->line))
						{
							char buf[16];

							cdev->write(sline_rightpart(&rl->line), sline_rightsize(&rl->line), 0);
							ret = vt100_left(buf, sline_rightsize(&rl->line));
							cdev->write(buf, ret, 0);
						}
						break;
					}
					

					case READLINE_RIGHT:
						cdev->write(VT100_RIGHT, 3, 0);
						break;

					case READLINE_LEFT:
						cdev->write(VT100_LEFT, 3, 0);
						break;

					case READLINE_NOTHING:
						break;

					case READLINE_UPDATELINE:
					{
						char buf[16];

						if (rl->lastsize)
						{
							ret = vt100_left(buf, rl->lastsize);
							cdev->write(buf, ret, 0);
							cdev->write(VT100_ERASE_LINE_AFTER_CURSOR, 3, 0);
						}
						if (rl->line.len)
							cdev->write(rl->line.buf, rl->line.len, 0);
						break;
					}

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