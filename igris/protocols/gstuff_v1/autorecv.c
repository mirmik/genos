#include "autorecv.h"
#include <igris/compiler.h>

void gstuff_autorecv_reset_v1(struct gstuff_autorecv_v1 *autom)
{
    autom->crc = 0xff;
    sline_reset(&autom->line);
}

void gstuff_autorecv_setbuf_v1(struct gstuff_autorecv_v1 *autom,
                               void *buf,
                               int len)
{
    sline_init(&autom->line, buf, len);
    gstuff_autorecv_reset_v1(autom);
}

int gstuff_autorecv_newchar_v1(struct gstuff_autorecv_v1 *autom, char c)
{
    int sts;

    switch (autom->state)
    {
    case 0:
        gstuff_autorecv_reset_v1(autom);

        // goto state 1 imediatly;
        autom->state = 1;
        IGRIS_FALLTHROUGH

    case 1:
        switch (c)
        {
        case GSTUFF_START_V1:
            //Приняли стартовый символ.
            if (sline_empty(
                    &autom->line)) //< Повторный стартовый. Ничего не делаем.
                goto __continue__;

            if (autom->crc != 0)
            {
                //Принят символ окончания пакета, но crc не пройден.
                sts = GSTUFF_CRC_ERROR_V1;
                goto __finish__;
            }

            else
            {
                //Корректный приём пакета.
                sts = GSTUFF_NEWPACKAGE_V1;
                goto __finish__;
            }

        case GSTUFF_STUB_V1:
            //Принят STUFF ждем вторй байт.
            autom->state = 2;
            goto __continue__;

        default:
            goto __putchar__;
        }

        // unreachable point

    case 2:
        // На прошлой итерации принят STUFF.
        // Обрабатываем второй символ.
        switch (c)
        {
        case GSTUFF_STUB_START_V1:
            c = GSTUFF_START_V1;
            break;
        case GSTUFF_STUB_STUB_V1:
            c = GSTUFF_STUB_V1;
            break;
        default:
            // Невалидный пакет.
            sts = GSTUFF_DATA_ERROR_V1;
            goto __finish__;
        }

        goto __putchar__;
    }

__putchar__:
    if (!sline_putchar(&autom->line, c))
    {
        sts = GSTUFF_OVERFLOW_V1;
        goto __finish__;
    }
    igris_strmcrc8(&autom->crc, c);
    autom->state = 1;

// fallthrow
__continue__:
    return GSTUFF_CONTINUE_V1;

__finish__:
    autom->state = 0;
    return sts;
}
