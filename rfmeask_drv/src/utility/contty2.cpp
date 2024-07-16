#include <cassert>
#include <errno.h>
#include <igris/defs/vt100.h>
#include <igris/dprint.h>
#include <igris/shell/readline.h>
#include <igris/util/bug.h>
#include <utility/contty2.h>
#include <utility/mshell.h>

static int newline(struct contty2_context *cntxt)
{
    int ret;
    cntxt->rl.line.buf[cntxt->rl.line.len] = 0;
    ret = mshell_execn(
        cntxt->rl.line.buf, cntxt->rl.line.len, SH_INTERNAL_SPLIT, NULL, 1);

    switch (ret)
    {
    case ENOENT:
        cntxt->cdev->write("Entry not found\n\r", 17);
        cntxt->cdev->write("message: ", 9);
        cntxt->cdev->write(cntxt->rl.line.buf, cntxt->rl.line.len);
        cntxt->cdev->write("\r\n", 2);
        break;
    }

    return ret;
}

void contty2_debug_mode(struct contty2_context *cntxt, int en)
{
    cntxt->debug_mode = en;
}

void contty2_automate(void *arg, int *state)
{
    // struct file * filp;
    char c;
    int ret;

    struct contty2_context *cntxt = (struct contty2_context *)arg;
    genos::chardev *cdev = cntxt->cdev;
    struct readline *rl = &cntxt->rl;

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
        cdev->write("$ ", 2);

    case 2:
        if (cntxt->debug_mode)
            dprln("contty: state 2");

        *state = 3;
        cdev->wait_for_avail(); //Неблокирующий wait для автомата.
        break;

    case 3:
        if (cntxt->debug_mode)
            dprln("contty: state 3");

        ret = cdev->read(&c, 1);
        {
            ret = readline_putchar(rl, c);

            switch (ret)
            {
            case READLINE_ECHOCHAR:
            {
                cdev->write(&c, 1);

                if (!sline_in_rightpos(&rl->line))
                {
                    char buf[16];

                    cdev->write(sline_rightpart(&rl->line),
                                sline_rightsize(&rl->line));
                    ret = vt100_left(buf, sline_rightsize(&rl->line));
                    cdev->write(buf, ret);
                }
            }
            break;

            case READLINE_NEWLINE:
                *state = 1;
                cdev->write("\r\n", 2);
                newline(cntxt);
                return;

            case READLINE_BACKSPACE:
            {
                cdev->write(VT100_LEFT, 3);
                cdev->write(VT100_ERASE_LINE_AFTER_CURSOR, 3);

                if (!sline_in_rightpos(&rl->line))
                {
                    char buf[16];

                    cdev->write(sline_rightpart(&rl->line),
                                sline_rightsize(&rl->line));
                    ret = vt100_left(buf, sline_rightsize(&rl->line));
                    cdev->write(buf, ret);
                }
                break;
            }

            case READLINE_RIGHT:
                cdev->write(VT100_RIGHT, 3);
                break;

            case READLINE_LEFT:
                cdev->write(VT100_LEFT, 3);
                break;

            case READLINE_NOTHING:
                break;

            case READLINE_UPDATELINE:
            {
                char buf[16];

                if (rl->lastsize)
                {
                    ret = vt100_left(buf, rl->lastsize);
                    cdev->write(buf, ret);
                    cdev->write(VT100_ERASE_LINE_AFTER_CURSOR, 3);
                }
                if (rl->line.len)
                    cdev->write(rl->line.buf, rl->line.len);
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
}