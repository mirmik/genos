/**
@file
Драйвер строки ввода. Реализует обработку поведения курсора, операций вставок
и удалений, специфических для работы с редактируемым пользовательским вводом.
*/

#ifndef IGRIS_DATASTRUCT_SLINE_H
#define IGRIS_DATASTRUCT_SLINE_H

#include <igris/compiler.h>
#include <igris/dprint.h>
#include <string.h>

struct sline
{
    char *buf;
    unsigned int cap;
    unsigned int len;
    unsigned int cursor;
};

__BEGIN_DECLS

static inline const char *sline_getline(struct sline *sl)
{
    sl->buf[sl->len] = '\0';
    return sl->buf;
}

static inline char *sline_rightpart(struct sline *sl)
{
    return sl->buf + sl->cursor;
}

static inline unsigned int sline_rightsize(struct sline *sl)
{
    return sl->len - sl->cursor;
}

static inline unsigned int sline_in_rightpos(struct sline *sl)
{
    return sl->len == sl->cursor;
}

static inline void sline_reset(struct sline *sl)
{
    sl->len = 0;
    sl->cursor = 0;
}

static inline int sline_equal(struct sline *sl, const char *str)
{
    if (sl->len != strlen(str))
        return 0;

    return strncmp(sl->buf, str, sl->len) == 0;
}

static inline int sline_left(struct sline *sl)
{
    if (sl->cursor == 0)
        return 0;
    --sl->cursor;
    return 1;
}

static inline int sline_right(struct sline *sl)
{
    if (sl->cursor == sl->len)
        return 0;
    ++sl->cursor;
    return 1;
}

static inline void
sline_setbuf(struct sline *sl, char *buffer, unsigned int bufcap)
{
    sl->buf = buffer;
    sl->cap = bufcap;
}

static inline void
sline_init(struct sline *sl, char *buffer, unsigned int bufcap)
{
    sline_setbuf(sl, buffer, bufcap);
    sline_reset(sl);
}

static inline int sline_backspace(struct sline *sl, unsigned int count)
{
    if (count > sl->cursor)
        count = sl->cursor;

    sl->len -= count;
    sl->cursor -= count;

    if (sl->cursor != sl->len)
    {
        memmove(sl->buf + sl->cursor,
                sl->buf + sl->cursor + count,
                sl->len - sl->cursor);
    }

    return (int)count;
}

static inline int sline_delete(struct sline *sl, unsigned int count)
{
    if (count > sline_rightsize(sl))
        count = sline_rightsize(sl);

    sl->len -= count;

    if (sl->cursor != sl->len)
    {
        memmove(sl->buf + sl->cursor,
                sl->buf + sl->cursor + count,
                sl->len - sl->cursor);
    }

    return (int)count;
}

static inline int sline_empty(struct sline *sl)
{
    return sl->len == 0;
}

static inline int sline_avail(struct sline *sl)
{
    return sl->cap - sl->len;
}

static inline int sline_size(struct sline *sl)
{
    return sl->len;
}

static inline int sline_putchar(struct sline *sl, char c)
{
    if (sl->len >= sl->cap - 1)
        return 0;

    if (sl->cursor != sl->len)
    {
        memmove(sl->buf + sl->cursor + 1,
                sl->buf + sl->cursor,
                sl->len - sl->cursor);
    }

    sl->buf[sl->cursor++] = c;
    sl->len++;

    return 1;
}

static inline int sline_newdata(struct sline *sl, const char *data, int len)
{
    if (len > sline_avail(sl))
        len = sline_avail(sl);

    if (sl->cursor != sl->len)
    {
        memmove(sl->buf + sl->cursor + len,
                sl->buf + sl->cursor,
                sl->len - sl->cursor);
    }

    memcpy(sl->buf + sl->cursor, data, len);
    sl->cursor += len;
    sl->len += len;

    return len;
}

__END_DECLS

#endif
