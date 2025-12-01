/**
@file
Расширение драйвера datastruct/sline, реализующее логику работы командной строки
в парадигме readline. Драйвер умеет преобразовывать управляющие входные
последовательности в команды sline и реализует механизм работы с историей
предыдущих команд.

Драйвер реализован в виде callback-less машины состояний и предполагает
наличие внешнего уровня, интерпретирующего возвращаемые машиной статусы.

TODO: Вынести драйвер работы с историей как отдельный объект.
*/

#ifndef IGRIS_SHELL_READLINE_H
#define IGRIS_SHELL_READLINE_H

#include <igris/datastruct/sline.h>
#include <igris/defs/ascii.h>
#include <igris/util/bug.h>

#include <igris/dtrace.h>

#define READLINE_OVERFLOW -1

#define READLINE_NOTHING 0
#define READLINE_ECHOCHAR 1

#define READLINE_NEWLINE 2
#define READLINE_BACKSPACE 3
#define READLINE_DELETE 4
#define READLINE_HOME 5
#define READLINE_END 6

#define READLINE_UPDATELINE 7

#define READLINE_LEFT 8
#define READLINE_RIGHT 9

#define READLINE_STATE_NORMAL 0
#define READLINE_STATE_ESCSEQ 1
#define READLINE_STATE_ESCSEQ_MOVE 2
#define READLINE_STATE_ESCSEQ_MOVE_WAIT_7E 3

struct readline
{
    struct sline line;
    int state;

    char last;
    int lastsize;

    char *history_space; // Указатель на буффер истории.
    uint8_t history_size; // Количество строк в буффере истории.

    uint8_t headhist; // Индекс в массиве, куда будет перезаписываться новая
                      // строка истории.
    uint8_t curhist; // Индекс выбора строки истории (0-пустая, 1-последняя,
                     // 2-предпоследняя и т.д.)
};

__BEGIN_DECLS

static inline void readline_init(struct readline *rl, char *buf, size_t len)
{
    sline_init(&rl->line, buf, (unsigned int)len);

    rl->last = 0;
    rl->state = 0;
    rl->history_space = NULL;
    rl->curhist = 0;
    rl->headhist = 0;
}

static inline void
readline_history_init(struct readline *rl, char *hs, int hsize)
{
    rl->history_space = hs;
    rl->history_size = hsize;
    memset(hs, 0, rl->line.cap * hsize);
}

static inline void readline_newline_reset(struct readline *rl)
{
    // Когда случается новая линия, сбрасываем буффер ввода и скидываем
    // переключатель строки истории на последнюю строку.
    sline_reset(&rl->line);
    rl->curhist = 0;
}

// Указатель на строку, взятую от последней пришедшей. (1 - последняя)
static inline char *readline_history_pointer(struct readline *rl, int num)
{
    int idx = (rl->headhist + rl->history_size - num) % rl->history_size;
    return rl->history_space + idx * rl->line.cap;
}

// Вернуть указатель на строку истории, на которую указывает curhist.
// Вызывается на кнопки вверх/вниз
static inline char *readline_current_history_pointer(struct readline *rl)
{
    return readline_history_pointer(rl, rl->curhist);
}

static inline void
_readline_push_line_to_history(struct readline *rl, const char *str, size_t len)
{
    char *ptr = rl->history_space + rl->headhist * rl->line.cap;
    memcpy(ptr, str, len);
    *(ptr + len) = '\0';
    rl->headhist = (rl->headhist + 1) % rl->history_size;
}

static inline void readline_push_line_to_history(struct readline *rl,
                                                 const char *str)
{
    _readline_push_line_to_history(rl, str, strlen(str));
}

// Обновить историю, записав туда новую строку.
static inline void readline_push_current_line_to_history(struct readline *rl)
{
    _readline_push_line_to_history(rl, rl->line.buf, rl->line.len);
}

static inline void readline_load_history_line(struct readline *rl)
{
    rl->lastsize = rl->line.len;

    if (rl->curhist == 0)
    {
        memset(rl->line.buf, 0, rl->line.cap);
        rl->line.len = 0;
        rl->line.cursor = 0;
        return;
    }

    unsigned int sz =
        (unsigned int)strlen(readline_current_history_pointer(rl));

    memcpy(rl->line.buf, readline_current_history_pointer(rl), sz);
    rl->line.len = rl->line.cursor = sz;
}

static inline int readline_history_up(struct readline *rl)
{
    // DTRACE();
    if (rl->history_space == NULL)
        return 0;

    if (rl->curhist == rl->history_size)
        return 0;

    rl->curhist++;

    readline_load_history_line(rl);
    return 1;
}

static inline int readline_is_not_same_as_last(struct readline *rl)
{
    return !sline_equal(&rl->line, readline_history_pointer(rl, 1));
}

static inline int readline_history_down(struct readline *rl)
{
    if (rl->history_space == NULL)
        return 0;

    if (rl->curhist == 0)
        return 0;

    rl->curhist--;

    readline_load_history_line(rl);
    return 1;
}

static inline int readline_putchar(struct readline *rl, char c)
{
    int ret;
    int retcode;

    switch (rl->state)
    {
    case READLINE_STATE_NORMAL:
        switch (c)
        {
        case '\r':
        case '\n':
            // TODO: Возможно тут некорректно отрабатывается комбинация rnrnrnrn
            if ((rl->last == '\n' || rl->last == '\r') && rl->last != c)
            {
                rl->last = 0;
                retcode = READLINE_NOTHING;
            }
            else
            {
                if (rl->history_space && rl->line.len &&
                    readline_is_not_same_as_last(rl))
                {
                    // Если есть буффер истории и введенная строка не нулевая и
                    // отличается от последней, сохраняем строку в историю.
                    readline_push_current_line_to_history(rl);
                }
                rl->curhist = 0;

                retcode = READLINE_NEWLINE;
            }
            break;

        case ASCII_BS:
            ret = sline_backspace(&rl->line, 1);
            retcode = ret ? READLINE_BACKSPACE : READLINE_NOTHING;
            break;

        case ASCII_ESC:
            rl->state = READLINE_STATE_ESCSEQ;
            retcode = READLINE_NOTHING;
            break;

        default:
            sline_putchar(&rl->line, c);
            retcode = READLINE_ECHOCHAR;
            break;
        }
        break;

    case READLINE_STATE_ESCSEQ:
        switch (c)
        {
        case 0x5B:
            rl->state = READLINE_STATE_ESCSEQ_MOVE;
            break;

        default:
            // dprln("?warning in readline?");
            rl->state = READLINE_STATE_NORMAL;
            break;
        }
        retcode = READLINE_NOTHING;
        break;

    case READLINE_STATE_ESCSEQ_MOVE:
        retcode = READLINE_NOTHING;
        switch (c)
        {
        case 0x41: // up
            if (readline_history_up(rl))
                retcode = READLINE_UPDATELINE;
            break;
        case 0x42: // down
            if (readline_history_down(rl))
                retcode = READLINE_UPDATELINE;
            break;
        case 0x43: // right
            ret = sline_right(&rl->line);
            if (ret)
                retcode = READLINE_RIGHT;
            break;
        case 0x44: // left
            ret = sline_left(&rl->line);
            if (ret)
                retcode = READLINE_LEFT;
            break;
        case 0x33:
            ret = sline_delete(&rl->line, 1);
            if (ret)
                retcode = READLINE_DELETE;
            rl->state = READLINE_STATE_ESCSEQ_MOVE_WAIT_7E;
            rl->last = c;
            return retcode;
        default:
            // dpr("unr esc: hex:");
            // dprhex(c);
            // dpr(" dec:");
            // dprln((int)c);
            ;
        }
        rl->state = READLINE_STATE_NORMAL;
        break;

    case READLINE_STATE_ESCSEQ_MOVE_WAIT_7E:
        // if (c != '\x7E')
        //    dprln("?esc?");
        rl->state = READLINE_STATE_NORMAL;
        retcode = READLINE_NOTHING;
        break;

    default:
        // автомат находится в непредусмотренном состоянии
        // сбрасываем состояние и завершаем автомат.
        rl->state = READLINE_STATE_NORMAL;
        retcode = READLINE_NOTHING;
    }

    rl->last = c;
    return retcode;
}

static inline int
readline_linecpy(struct readline *rl, char *line, size_t maxlen)
{
    int len = (int)maxlen - 1 > (int)rl->line.len ? (int)rl->line.len
                                                  : (int)maxlen - 1;

    memcpy(line, rl->line.buf, len);
    line[len] = 0;

    return len;
}

__END_DECLS

#endif
