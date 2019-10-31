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
	int ret;
	cntxt->rl.line.buf[cntxt->rl.line.len] = 0;
	ret = mshell_execn(cntxt->rl.line.buf, cntxt->rl.line.len, SH_INTERNAL_SPLIT, NULL, 1);

	switch (ret)
	{
		case ENOENT:
			write(STDOUT_FILENO, "Entry not found\n\r", 17);
			write(STDOUT_FILENO, "message: ", 9);
			write(STDOUT_FILENO, cntxt->rl.line.buf, cntxt->rl.line.len);
			write(STDOUT_FILENO, "\r\n", 2);
			break;
	}

	return ret;
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
	//struct serial_device * cdev = cntxt->cdev;
	struct readline * rl = &cntxt->rl;

	switch (*state)
	{
		case 0:
			open(cntxt->iopath, 0);
			open(cntxt->iopath, 0);

			if (cntxt->debug_mode)
				dprln("contty: state 0");

			readline_init(rl, cntxt->buffer, CONTTY3_LINE_LENGTH);
			readline_history_init(rl, cntxt->hbuffer, CONTTY3_HISTORY_SIZE);
			*state = 1;

		case 1:
			if (cntxt->debug_mode)
				dprln("contty: state 1");

			readline_newline_do(rl);
			*state = 2;
			write(STDOUT_FILENO, "$ ", 2);

		case 2:
			if (cntxt->debug_mode)
				dprln("contty: state 2");

			*state = 3;
			read(STDIN_FILENO, &c, 0); //Неблокирующий wait для автомата.
			break;

		case 3:
			if (cntxt->debug_mode)
				dprln("contty: state 3");

			while ((ret = read(STDIN_FILENO, &c, 1)))
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
						write(STDOUT_FILENO, &c, 1);

						if (!sline_in_rightpos(&rl->line))
						{
							char buf[16];

							write(STDOUT_FILENO, sline_rightpart(&rl->line), sline_rightsize(&rl->line));
							ret = vt100_left(buf, sline_rightsize(&rl->line));
							write(STDOUT_FILENO, buf, ret);
						}
					}
					break;

					case READLINE_NEWLINE:
						*state = 1;
						write(STDOUT_FILENO, "\r\n", 2);
						newline(cntxt);
						return NULL;

					case READLINE_BACKSPACE:
					{
						write(STDOUT_FILENO, VT100_LEFT, 3);
						write(STDOUT_FILENO, VT100_ERASE_LINE_AFTER_CURSOR, 3);

						if (!sline_in_rightpos(&rl->line))
						{
							char buf[16];

							write(STDOUT_FILENO, sline_rightpart(&rl->line), sline_rightsize(&rl->line));
							ret = vt100_left(buf, sline_rightsize(&rl->line));
							write(STDOUT_FILENO, buf, ret);
						}
						break;
					}
					

					case READLINE_RIGHT:
						write(STDOUT_FILENO, VT100_RIGHT, 3);
						break;

					case READLINE_LEFT:
						write(STDOUT_FILENO, VT100_LEFT, 3);
						break;

					case READLINE_NOTHING:
						break;

					case READLINE_UPDATELINE:
					{
						char buf[16];

						if (rl->lastsize)
						{
							ret = vt100_left(buf, rl->lastsize);
							write(STDOUT_FILENO, buf, ret);
							write(STDOUT_FILENO, VT100_ERASE_LINE_AFTER_CURSOR, 3);
						}
						if (rl->line.len)
							write(STDOUT_FILENO, rl->line.buf, rl->line.len);
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