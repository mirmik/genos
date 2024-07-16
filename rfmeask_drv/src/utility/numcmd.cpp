#include <igris/sync/syslock.h>
#include <igris/util/bug.h>
#include <igris/util/hexascii.h>
#include <igris/util/retype.h>
#include <utility/numcmd.h>

void numcmd_send_answer(struct numcmd_context *cntxt, int32_t ret)
{
    char buf[14];
    buf[0] = '\002';
    buf[13] = '\003';

    uint8_to_hex(buf + 1, 255);
    uint32_to_hex(buf + 3, ret);

    numcmd_form_checksum(buf);

    system_lock();
    cntxt->serdev->write(buf, 14);
    system_unlock();

    if (cntxt->debug_mode)
    {
        dpr("numcmd: answer:");
        debug_write(buf, 14);
        dln();
    }
}

void numcmd_parse_answer(struct numcmd_context *cntxt, const char *str)
{
    if (cntxt->debug_mode)
        dprln("numcmd: parse answer");
}

void numcmd_parse_message(struct numcmd_context *cntxt, const char *str)
{
    uint32_t ret;
    int sts;

    if (cntxt->debug_mode)
        dprln("numcmd: parse message");

    sts = numcmd_do(str, cntxt->numcmd_table, &ret);

    if (sts)
    {
        if (cntxt->debug_mode)
            dprln("numcmd: enoent");
        return;
    }
    else
        numcmd_send_answer(cntxt, ret);
}

void numcmd_wrong_frame(struct numcmd_context *cntxt)
{
    if (cntxt->debug_mode)
        dprln("numcmd: wrong frame");

    cntxt->serdev->write("\001JJ\003", 4);
};

void numcmd_newmessage(struct numcmd_context *cntxt)
{
    if (cntxt->debug_mode)
        dprln("numcmd: receive");

    const char *str = sline_getline(&cntxt->line);

    if (!numcmd_checksum_check(str))
    {
        if (cntxt->debug_mode)
            dprln("numcmd: uncorrect checksum");
        else
            dprln("numcmd: checksum correct");

        numcmd_wrong_frame(cntxt);
        return;
    };

    if (*str != 1 && *str != 2)
    {
        numcmd_wrong_frame(cntxt);
        return;
    };

    if (*str == 2)
        numcmd_parse_answer(cntxt, str);

    if (*str == 1)
        numcmd_parse_message(cntxt, str);
}

void numcmd_error(struct numcmd_context *cntxt)
{
    if (cntxt->debug_mode)
        dprln("numcmd: error");
}

// UNSUPORTED
void numcmd_resend(struct numcmd_context *cntxt)
{
    if (cntxt->debug_mode)
        dprln("numcmd: resend");

    numcmd_error(cntxt);
}

int i = 0;
void numcmd_automate(void *arg, int *state)
{
    RETYPE(struct numcmd_context *, cntxt, arg);

    char c;
    genos::chardev *serdev = cntxt->serdev;
    struct sline *line = &cntxt->line;

    switch (cntxt->state)
    {
    case 0:
        if (cntxt->debug_mode)
            dprln("numcmd: state 0");

        sline_setbuf(line, cntxt->buffer, NUMCMD_LINE_LENGTH);
        cntxt->state = 1;
        break;

    case 1:
        if (cntxt->debug_mode)
            dprln("numcmd: state 1");

        sline_reset(line);
        cntxt->state = 2;

        break;

    case 2:
        if (cntxt->debug_mode)
            dprln("numcmd: state 2");

        // dpr("line_state: "); debug_write(line->buf, line->len); dprln();
        serdev->wait_for_avail(); //Неблокирующий wait для автомата.
        cntxt->state = 3;
        break;

    case 3:
        if (cntxt->debug_mode)
            dprln("numcmd: state 3");
        {
            serdev->read(&c, 1);
            if (cntxt->debug_mode)
            {
                dpr("numcmd: read char ");
                dprchar(c);
                dln();
            }

            switch (c)
            {
            case '\x01':
                sline_reset(line);
                sline_putchar(line, c);
                break;

            case '\x03':
                sline_putchar(line, c);

                if (line->len == 14)
                {
                    numcmd_newmessage(cntxt);
                    sline_reset(line);
                    break;
                }

                else if (line->len == 4)
                {
                    numcmd_resend(cntxt);
                    sline_reset(line);
                    break;
                }

                else
                {
                    numcmd_error(cntxt);
                    sline_reset(line);
                    break;
                }

                break;

            default:
                sline_putchar(line, c);
                break;
            }
        }

        cntxt->state = 2;
        break;

    default:
        BUG();
    }
}
