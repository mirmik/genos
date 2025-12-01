#ifndef IGRIS_DATASTRUCT_ARGV_H
#define IGRIS_DATASTRUCT_ARGV_H

#include <stddef.h>
#include <string.h>

// Вычислить длину первого слова строки.
static inline ptrdiff_t argvc_length_of_first(const char *str)
{
    const char *strt = str;

    while (*str != ' ' && *str != '\0')
    {
        ++str;
    }

    return str - strt;
}

// Выполняет разбиение строки data, терминированной нулём
// Разбиение выполняется путём записи нулей в тело строки,
// вектор результата возвращается в массив argv,
// Максимальное количество элементов вектора argcmax
static inline int argvc_internal_split(char *data, char **argv, int argcmax)
{
    const char *ws = " \r\n\t";

    int argc = 0;

    while (1)
    {
        while (*data != '\0')
        {
            if (strchr(ws, *data))
                ++data;
            else
                break;
        }
        if (*data == '\0' || argc >= argcmax)
            return argc;

        argv[argc++] = data;
        while (!strchr(ws, *data) && *data != '\0')
            ++data;

        if (*data == '\0')
            return argc;

        if (strchr(ws, *data))
        {
            *data++ = '\0';
            continue;
        };
        break;
    }

    return argc;
}

// Безопасный вариант функции argvc_internal_split, дополнительно
// проверяющий длину строки (может не быть терминирована).
static inline int
argvc_internal_split_n(char *data, int maxlen, char **argv, int argcmax)
{
    const char *ws = " \r\n\t";
    int argc = 0;
    char *eptr = data + maxlen;

newarg_search:
    while (strchr(ws, *data) && data != eptr)
        ++data;
    if (*data == '\0' || argc >= argcmax || data == eptr)
        return argc;

    argv[argc++] = data;
    while (!strchr(ws, *data) && data != eptr)
        ++data;
    if (strchr(ws, *data))
    {
        *data++ = '\0';
        goto newarg_search;
    };

    return argc;
}

#endif
