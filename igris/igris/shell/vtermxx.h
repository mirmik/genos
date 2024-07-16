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
#include <igris/event/delegate.h>
#include <igris/shell/readlinexx.h>

#define VTERMXX_INIT_STEP (-1)

namespace igris
{
    class vtermxx
    {
        igris::delegate<void, const char *, unsigned int> execute_callback = {};
        igris::delegate<void, const char *, unsigned int> write_callback = {};
        igris::delegate<void, int> signal_callback = {};

        int state = 0;
        uint8_t echo = 1;
        const char *prefix_string = {};
        igris::readline rl = {};

    public:
        void init(unsigned int buffer_size, unsigned int history_size);

        void newdata(int16_t c);

        void set_prompt(const char *str)
        {
            prefix_string = str;
        }

        void set_echo(uint8_t val)
        {
            echo = val;
        }

        void set_write_callback(
            igris::delegate<void, const char *, unsigned int> callback)

        {
            write_callback = callback;
        }

        void set_execute_callback(
            igris::delegate<void, const char *, unsigned int> callback)

        {
            execute_callback = callback;
        }

        void set_signal_callback(igris::delegate<void, int> callback)
        {
            signal_callback = callback;
        }

        void init_step()
        {
            newdata(VTERMXX_INIT_STEP);
        }

    private:
        void newline();
    };
}

#endif
