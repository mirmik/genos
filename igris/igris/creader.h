#ifndef IGRIS_CREADER_H
#define IGRIS_CREADER_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <igris/compiler.h>

struct creader
{
    const char *strt;
    const char *fini;

    const char *cursor;
};

__BEGIN_DECLS

static inline void
creader_init(struct creader *reader, const char *strt, size_t size)
{
    reader->strt = reader->cursor = strt;
    reader->fini = strt + size;
}

static inline uint8_t creader_end(struct creader *reader)
{
    return reader->cursor == reader->fini;
}

static inline size_t creader_curpos(struct creader *reader)
{
    return (size_t)(reader->cursor - reader->strt);
}

static inline size_t creader_itpos(struct creader *reader, const char *it)
{
    return (size_t)(it - reader->strt);
}

static inline ptrdiff_t creader_readline(struct creader *reader,
                                         const char **token)
{
    ptrdiff_t len;
    const char *it = reader->cursor;
    *token = reader->cursor;

    if (creader_end(reader))
    {
        return -1;
    }

    // Шагаем в конец строки.
    while (*it != '\n' && *it != '\0' && it != reader->fini)
        it++;

    // Обновляем положение курсора.
    if (it != reader->fini)
    {
        reader->cursor = it + 1;
    }
    else
    {
        return it - *token;
    }

    // Отматываем назад, чтобы корректно расчитать расстояние.
    while ((it != *token) && (*it == '\n' || *it == '\r'))
        --it;

    if (it == *token)
        return 0;

    len = it - *token + 1;
    return len;
}

static inline int creader_skip(struct creader *reader, const char *symbols)
{
    int count = 0;

    while (reader->cursor != reader->fini)
    {
        int found = 0;

        for (const char *s = symbols; *s != 0; ++s)
        {
            if (*reader->cursor == *s)
            {
                found = 1;
                break;
            }
        }

        if (found)
        {
            count++;
            reader->cursor++;
        }

        else
        {
            break;
        }
    }

    return count;
}

static inline int creader_skipws(struct creader *reader)
{
    return creader_skip(reader, "\t\n\r ");
}

__END_DECLS

#endif
