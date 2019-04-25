#include <utility/contty.h>

#include <drivers/cdev/cdev.h>
#include <igris/util/bug.h>
#include <igris/dprint.h>

#include <errno.h>
#include <utility/mshell.h>

static int newline(struct contty_context * cntxt)
{
	int ret;

//	struct schedee * sch = create_autom_schedee(sh_utility, NULL);
//	schedee_run(sch);
//	__schedee_wait_for(current_schedee(), sch);
//	__displace__();
	ret = mshell_execn(cntxt->line.buf, cntxt->line.len, SH_INTERNAL_SPLIT, NULL);

	switch (ret)
	{
		case ENOENT: cntxt->cdev->c_ops->write(
			    cntxt->cdev, "Entry not found\n\r", 17, 0);
	}

	return ret;
}

void contty_debug_mode(struct contty_context * cntxt, int en)
{
	cntxt->debug_mode = en;
}

void * contty_automate(void * arg, int * state)
{
	struct file * filp;
	char c;
	int ret;

	struct contty_context * cntxt = (struct contty_context *) arg;
	struct char_device * cdev = cntxt->cdev;
	struct sline * line = &cntxt->line;

	switch (*state)
	{
		case 0:
			if (cntxt->debug_mode)
				dprln("contty: state 0");

			//filp = vfs_file_alloc();
			//cdev->open(filp);
			//vfs_open_node(cdev, &filp);
			//schedee_setfd(current_schedee(), filp, 0); //input
			//schedee_setfd(current_schedee(), filp, 1); //output

			cntxt->last = '\n';
			sline_setbuf(line, cntxt->buffer, CONTTY_LINE_LENGTH);
			*state = 1;

		case 1:
			if (cntxt->debug_mode)
				dprln("contty: state 1");

			sline_reset(line);
			cdev->c_ops->write(cdev, "$ ", 2, 0);
			*state = 2;

		case 2:
			if (cntxt->debug_mode)
				dprln("contty: state 2");

			//dpr("line_state: "); debug_write(line->buf, line->len); dprln();
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
					dprchar(c);
					dln();
				}

				switch (c)
				{
					case '\r':
					case '\n':
						if (cntxt->last == '\r')
							break;
						cntxt->last = '\n';
						cdev->c_ops->write(cdev, "\r\n", 2, 0);
						newline(cntxt);
						*state = 1;
						return NULL;

					default:
						cdev->c_ops->write(cdev, &c, 1, 0);
						sline_putchar(line, c);
						break;
				}
				cntxt->last = c;
			}
			*state = 2;
			break;

		default:
			BUG();
	}

	return NULL;
}