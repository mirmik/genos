#include <igris/protocols/gstuff.h>
#include <igris/util/crc.h>
#include <vector>

int gstuffing(const char *data, size_t size, char *outdata)
{
    struct iovec vec[] = {{(void *)data, size}};
    return gstuffing_v(vec, 1, outdata);
}

int gstuff_byte(char c, char *outdata)
{
    switch (c)
    {
    case GSTUFF_START:
        *outdata++ = GSTUFF_STUB;
        *outdata++ = GSTUFF_STUB_START;
        return 2;
    case GSTUFF_STOP:
        *outdata++ = GSTUFF_STUB;
        *outdata++ = GSTUFF_STUB_STOP;
        return 2;
    case GSTUFF_STUB:
        *outdata++ = GSTUFF_STUB;
        *outdata++ = GSTUFF_STUB_STUB;
        return 2;
    default:
        *outdata++ = c;
        return 1;
    }
}

int gstuffing_v(struct iovec *vec, size_t n, char *outdata)
{
    char *outstrt = outdata;
    uint8_t crc = 0xFF;
    *outdata++ = GSTUFF_START;
    for (size_t j = 0; j < n; ++j)
    {
        size_t size = vec[j].iov_len;
        char *data = (char *)vec[j].iov_base;
        while (size--)
        {
            char c = *data++;
            igris_strmcrc8(&crc, c);
            outdata += gstuff_byte(c, outdata);
        }
    }
    outdata += gstuff_byte(crc, outdata);
    *outdata++ = GSTUFF_STOP;
    return (int)(outdata - outstrt);
}

void gstuff_autorecv::init(uint8_t *buf, int len)
{
    this->state = 0;
    sline_init(&this->line, (char *)buf, len);
    reset();
}

void gstuff_autorecv::reset()
{
    this->crc = 0xff;
    sline_reset(&this->line);
}

int gstuff_autorecv::newchar(char c)
{
    int sts;

__start__:
    switch (state)
    {
    case 0:
        reset();
        state = 4;
        //__attribute__((fallthrough));
        goto __start__;

    case 4:
        switch (c)
        {
        case GSTUFF_START:
            //Приняли стартовый символ. Реинициализация.
            state = 1;
            reset();
            goto __continue__;

        default:
            goto __garbage__;
        }

    case 1:
        switch (c)
        {
        case GSTUFF_START:
            //Приняли стартовый символ. Реинициализация.
            reset();
            goto __force_restart__;

        case GSTUFF_STOP:
            // приняли финальный символ
            goto __stop_handler__;

        case GSTUFF_STUB:
            //Принят STUFF ждем вторй байт.
            state = 2;
            goto __continue__;

        default:
            goto __putchar__;
        }

    case 2:
        // На прошлой итерации принят STUFF.
        // Обрабатываем второй символ.
        switch (c)
        {
        case GSTUFF_STUB_START:
            c = GSTUFF_START;
            break;
        case GSTUFF_STUB_STOP:
            c = GSTUFF_STOP;
            break;
        case GSTUFF_STUB_STUB:
            c = GSTUFF_STUB;
            break;
        case GSTUFF_START:
            reset();
            goto __force_restart__;
        default:
            // Невалидный пакет.
            sts = GSTUFF_STUFFING_ERROR;
            goto __finish__;
        }

        goto __putchar__;
    }

    // unreacheble_point
    return GSTUFF_ALGORITHM_ERROR;

__stop_handler__:
    if (crc != 0)
    {
        //Принят символ окончания пакета, но crc не пройден.
        sts = GSTUFF_CRC_ERROR;
        goto __finish__;
    }

    else
    {
        //Корректный приём пакета. Удаляем crc символ
        sline_backspace(&line, 1);
        sts = GSTUFF_NEWPACKAGE;
        goto __finish__;
    }

__putchar__:
    if (!sline_putchar(&line, c))
    {
        sts = GSTUFF_OVERFLOW;
        state = 0;
        goto __finish__;
    }

    igris_strmcrc8(&crc, c);
    state = 1;

    // fallthrow
__continue__:
    return GSTUFF_CONTINUE;

__force_restart__:
    state = 1;
    return GSTUFF_FORCE_RESTART;

__garbage__:
    return GSTUFF_GARBAGE;

__finish__:
    state = 0;
    return sts;
}

std::vector<uint8_t> gstuffing_v(struct iovec *vec, size_t n)
{
    std::vector<uint8_t> ret;
    size_t sz = 0;
    for (size_t i = 0; i < n; ++i)
    {
        struct iovec *v = vec + i;
        sz += v->iov_len;
    }
    ret.resize(sz * 2 + 2);
    size_t sz2 = gstuffing_v(vec, n, (char *)&ret[0]);
    ret.resize(sz2);
    return ret;
}

std::vector<uint8_t> gstuffing(igris::buffer buf)
{
    std::vector<uint8_t> ret;
    ret.resize(buf.size() * 2 + 2);
    size_t sz2 = gstuffing(buf.data(), buf.size(), (char *)&ret[0]);
    ret.resize(sz2);
    return ret;
}
