/**
@file
Компонент vterm реализует алгоритм обработки терминального ввода в виде
автомата состояний и предназначен для реализации обработки терминального
ввода в составе какого-либо процесса обработчика, или в качестве
самостоятельного обработчика, если система не оперирует процессами.

Компонент основан на машине состояний igris/shell/readline.
*/

#ifndef IGRIS_SHELL_VTERM_H
#define IGRIS_SHELL_VTERM_H

#include <igris/compiler.h>
#include <igris/shell/readline.h>

#define VTERM_INIT_STEP (-1)

typedef void (*vterm_execute_t)(void *, const char *, unsigned int);
typedef void (*vterm_write_t)(void *, const char *, unsigned int);
typedef void (*vterm_signal_t)(void *, int);

struct vterm_automate
{
    vterm_execute_t execute_callback;
    vterm_write_t write_callback;
    vterm_signal_t signal_callback;

    void *execute_privdata;
    void *write_privdata;
    void *signal_privdata;

    int state;
    uint8_t echo;

    const char *prefix_string;

    struct readline rl;
};

__BEGIN_DECLS

/// @detail Длина буффера hbuffer = buffer_size * history_size
void vterm_automate_init(struct vterm_automate *vterm, char *buffer,
                         unsigned int buffer_size, char *hbuffer,
                         unsigned int history_size);

void vterm_automate_newdata(struct vterm_automate *vterm, int16_t c);

static inline void vterm_set_write_callback(struct vterm_automate *vterm,
                                            vterm_write_t callback,
                                            void *privdata)
{
    vterm->write_callback = callback;
    vterm->write_privdata = privdata;
}

static inline void vterm_set_execute_callback(struct vterm_automate *vterm,
                                              vterm_execute_t callback,
                                              void *privdata)
{
    vterm->execute_callback = callback;
    vterm->execute_privdata = privdata;
}

static inline void vterm_set_signal_callback(struct vterm_automate *vterm,
                                             vterm_signal_t callback,
                                             void *privdata)
{
    vterm->signal_callback = callback;
    vterm->signal_privdata = privdata;
}
static inline void vterm_automate_init_step(struct vterm_automate *vterm)
{
    vterm_automate_newdata(vterm, VTERM_INIT_STEP);
}

__END_DECLS

#endif
